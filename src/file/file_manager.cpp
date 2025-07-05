#include "file_manager.h"
#include "common.grpc.pb.h" //公共文件：包含服务类型等
#include "local_config.h"
#include <fstream>  // 文件流
#include <zip.h>    // libzip库
#include <filesystem>   // 文件系统库

FileManager::FileManager(GatewayManager& gateway_manager_, UserManager& user_manager_):
    logger_manager(),
    gateway_manager(gateway_manager_),
    user_manager(user_manager_)
{
    start_thread_pool(10);  // 启动线程池
    // 启动定时任务
    logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->info("File service started"); // 启动日志
}

FileManager::~FileManager()
{
    stop_thread_pool(); // 停止并清空线程池
    // 记录关闭日志
    logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->info("File service stopped");
    // 停止并清理日志管理器
    logger_manager.cleanup();
}

/************************************ 线程池工具函数 ****************************************************/
// 启动线程池
void FileManager::start_thread_pool(int num_threads)
{// 相关注释请参考 /src/central/src/central/central_server.cpp/start_thread_pool()
    for(int i = 0; i < num_threads; ++i)
    {
        this->thread_pool.emplace_back(&FileManager::Worker_thread, this);   // 创建线程
    }
}

// 停止线程池
void FileManager::stop_thread_pool()
{// 相关注释请参考 /src/central/src/central/central_server.cpp/stop_thread_pool()
    {
        std::lock_guard<std::mutex> lock(this->queue_mutex);
        this->stop_threads = true;
    }

    this->queue_cv.notify_all();

    for(auto& thread : this->thread_pool)
    {
        if(thread.joinable())
        {
            thread.join();
        }
    }
    this->thread_pool.clear();
    std::queue<std::function<void()>> empty;
    std::swap(this->task_queue, empty);
}

// 添加异步任务
std::future<void> FileManager::add_async_task(std::function<void()> task)
{
    auto task_ptr = std::make_shared<std::packaged_task<void()>>(std::move(task));
    std::future<void> task_future = task_ptr->get_future();

    {
        std::lock_guard<std::mutex> lock(this->queue_mutex);
        this->task_queue.push([task_ptr]() {
            (*task_ptr)();
        });
    }
    this->queue_cv.notify_one();
    return task_future;
}

// 线程池工作函数
void FileManager::Worker_thread()
{// 相关注释请参考 /src/central/src/central/central_server.cpp/worker_thread()
    while(true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);

            this->queue_cv.wait(lock, [this] {
                return !this->task_queue.empty() || this->stop_threads;
            });

            if(this->stop_threads && this->task_queue.empty())
            {
                return;
            }
            task = std::move(this->task_queue.front());
            this->task_queue.pop();
        }
        task();
    }
}

/************************************ gRPC服务接口 ********************************************************/
// 文件上传服务
void FileManager::Upload(std::string path)
{
    std::string zip_file_name;
    try
    {
        // 调用压缩函数，将文件打包压缩为 ZIP 文件
        zip_file_name = this->Compress_to_zip(path);
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("Path compressed to ZIP: {}", zip_file_name);
    }
    catch(const std::exception& e)
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("Compression failed: {}", e.what());
        return;
    }

    // 上传 ZIP 文件
    std::string account = this->user_manager.Get_current_account();
    std::string token = this->user_manager.Get_token(account);
    std::string file_server_address = "";
    std::string file_server_port = "";

    // 获取文件服务器地址和端口
    this->File_transmission_ready(zip_file_name, account, token, file_server_address, file_server_port);

    // 创建与文件服务器直连
    auto channel = grpc::CreateChannel(file_server_address + ":" + file_server_port, grpc::InsecureChannelCredentials());
    auto file_stub = rpc_server::FileServer::NewStub(channel);

    rpc_server::UploadRes res;
    grpc::ClientContext context;

    // 调用上传服务
    std::unique_ptr<grpc::ClientWriter<rpc_server::UploadReq>> writer(file_stub->Upload(&context, &res));

    std::ifstream zip_file(zip_file_name, std::ios::binary);
    if(!zip_file.is_open())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("Failed to open ZIP file: {}", zip_file_name);
        return;
    }

    // 写入上传数据流
    char buffer[1024];  // 缓冲区大小
    while(zip_file.read(buffer, sizeof(buffer)) || zip_file.gcount() > 0)
    {
        rpc_server::UploadReq request;
        request.set_account(account);
        request.set_file_name(zip_file_name);
        request.set_file_data(buffer, zip_file.gcount());
        if(!writer->Write(request))
        {
            break;
        }
    }

    // 关闭文件，请求流，结束上传
    zip_file.close();
    writer->WritesDone();
    grpc::Status status = writer->Finish();

    if(status.ok() && res.success())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("File upload success");
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("File upload failed: {}", status.error_message());
    }

    // 删除临时 ZIP 文件
    std::filesystem::remove(zip_file_name);
}

// 文件下载服务
void FileManager::Download(std::string file_name_)
{
    std::string account = user_manager.Get_current_account();
    std::string token = user_manager.Get_token(account);
    std::string file_server_address = "";
    std::string file_server_port = "";
    std::string save_die = LocalConfig::Get_config().dir_download; // 保存目录

    this->File_transmission_ready(file_name_, account, token, file_server_address, file_server_port);

    rpc_server::DownloadReq req;
    rpc_server::DownloadRes res;
    grpc::ClientContext context;

    req.set_account(account);
    req.set_file_name(file_name_);

    auto channel = grpc::CreateChannel(file_server_address + ":" + file_server_port, grpc::InsecureChannelCredentials());
    auto file_stub = rpc_server::FileServer::NewStub(channel);

    std::string zip_file_path = save_die + file_name_;
    std::ofstream zip_file(zip_file_path, std::ios::binary);
    if(!zip_file.is_open())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("Failed to open ZIP file: {}", zip_file_path);
        return;
    }

    std::unique_ptr<grpc::ClientReader<rpc_server::DownloadRes>> reader(file_stub->Download(&context, req));
    while(reader->Read(&res))
    {
        zip_file.write(res.file_data().data(), res.file_data().size());
    }

    grpc::Status status = reader->Finish();
    zip_file.close();

    if(!status.ok())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("File download failed: {}", status.error_message());
        return;
    }

    logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("ZIP file downloaded: {}", zip_file_path);

    // 解压 ZIP 文件
    int error = 0;
    zip_t* zip = zip_open(zip_file_path.c_str(), ZIP_RDONLY, &error);
    if(!zip)
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("Failed to open ZIP file: {}", zip_file_path);
        return;
    }

    std::string output_folder = save_die + file_name_.substr(0, file_name_.find_last_of('.'));
    std::filesystem::create_directories(output_folder);

    for(zip_int64_t i = 0; i < zip_get_num_entries(zip, 0); ++i)
    {
        const char* file_name = zip_get_name(zip, i, 0);
        if(!file_name) continue;

        std::string output_path = output_folder + "/" + file_name;
        zip_file_t* file = zip_fopen_index(zip, i, 0);
        if(!file) continue;

        std::ofstream out_file(output_path, std::ios::binary);
        char buffer[1024];
        zip_int64_t bytes_read;
        while((bytes_read = zip_fread(file, buffer, sizeof(buffer))) > 0)
        {
            out_file.write(buffer, bytes_read);
        }

        zip_fclose(file);
    }

    zip_close(zip);
    logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("ZIP file extracted to: {}", output_folder);
}

// 文件删除服务
void FileManager::Delete(std::string file_name_)
{
    std::string account = user_manager.Get_current_account();   // 用户账号
    std::string token = user_manager.Get_token(account);    // 用户token

    rpc_server::DeleteFileReq req;
    rpc_server::DeleteFileRes res;

    // 初始化请求
    req.set_account(account);
    req.file_name();

    // 通过网关转发，向服务器发送请求
    grpc::Status status = gateway_manager.Request_forward(&req, &res, rpc_server::ServiceType::REQ_FILE_DELETE);
    if(status.ok() && res.success())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("File delete success");
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("File delete failed");
    }
}

// 获取文件列表服务
void FileManager::ListFiles()
{
    std::string account = user_manager.Get_current_account();   // 用户账号
    std::string token = user_manager.Get_token(account);    // 用户token

    rpc_server::ListFilesReq req;
    rpc_server::ListFilesRes res;

    // 初始化请求
    req.set_account(account);

    // 通过网关转发，向服务器发送请求
    grpc::Status status = gateway_manager.Request_forward(&req, &res, rpc_server::ServiceType::REQ_FILE_LIST);
    if(status.ok() && res.success())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("File list success");
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("File list failed");
    }
}

/************************************ 定时任务 ********************************************************/

/************************************ 工具函数 **************************************************/
// 文件传输准备
void FileManager::File_transmission_ready(const std::string file_name_, const std::string account_, const std::string token_, std::string &file_server_address_, std::string &file_server_port_)
{
    // 文件传输准备（获取文件服务器地址）
    rpc_server::TransmissionReadyReq ready_req;
    rpc_server::TransmissionReadyRes ready_res;

    ready_req.set_account(account_);
    ready_req.set_token(token_);
    ready_req.set_file_name(file_name_);

    // 通过网关转发，向服务器发送请求
    grpc::Status ready_status = gateway_manager.Request_forward(&ready_req, &ready_res, rpc_server::ServiceType::REQ_FILE_TRANSMISSION_READY);
    if(ready_status.ok() && ready_res.success())
    {
        file_server_address_ = ready_res.file_server_address();
        file_server_port_ = ready_res.file_server_port();

        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("File transmission ready success");
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("File transmission ready failed");
        return;
    }
}

// 压缩文件或文件夹为 ZIP 文件
std::string FileManager::Compress_to_zip(const std::string& input_path)
{
    std::filesystem::path fs_path(input_path);

    // 检查路径是否存在
    if(!std::filesystem::exists(fs_path))
    {
        throw std::runtime_error("Path does not exist: " + input_path);
    }

    // 生成 ZIP 文件名
    std::string zip_file_name = fs_path.filename().string() + ".zip";

    // 创建 ZIP 文件
    int error = 0;
    zip_t* zip = zip_open(zip_file_name.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &error);
    if(!zip) {
        throw std::runtime_error("Failed to create ZIP file: " + zip_file_name);
    }

    try {
        if(std::filesystem::is_regular_file(fs_path)) {
            // 如果是文件，直接添加到 ZIP
            std::ifstream file(fs_path, std::ios::binary);
            if(!file.is_open()) {
                zip_close(zip);
                throw std::runtime_error("Failed to open file: " + fs_path.string());
            }

            std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            zip_source_t* source = zip_source_buffer(zip, file_content.data(), file_content.size(), 0);
            if(!source || zip_file_add(zip, fs_path.filename().string().c_str(), source, ZIP_FL_OVERWRITE) < 0) {
                zip_source_free(source);
                zip_close(zip);
                throw std::runtime_error("Failed to add file to ZIP: " + fs_path.string());
            }
        } else if(std::filesystem::is_directory(fs_path)) {
            // 如果是文件夹，递归添加文件到 ZIP
            for(const auto& entry : std::filesystem::recursive_directory_iterator(fs_path, std::filesystem::directory_options::skip_permission_denied)) {
                if(entry.is_regular_file()) {
                    std::ifstream file(entry.path(), std::ios::binary);
                    if(!file.is_open()) {
                        std::cerr << "Warning: Failed to open file: " << entry.path().string() << std::endl;
                        continue;
                    }

                    std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    std::string relative_path = std::filesystem::relative(entry.path(), fs_path).string();
                    zip_source_t* source = zip_source_buffer(zip, file_content.data(), file_content.size(), 0);
                    if(!source || zip_file_add(zip, relative_path.c_str(), source, ZIP_FL_OVERWRITE) < 0) {
                        zip_source_free(source);
                        std::cerr << "Warning: Failed to add file to ZIP: " << entry.path().string() << std::endl;
                    }
                }
            }
        } else {
            zip_close(zip);
            throw std::runtime_error("Unsupported path type: " + input_path);
        }
    } catch(const std::exception& e) {
        zip_close(zip);
        throw; // 重新抛出异常
    }

    zip_close(zip);
    return zip_file_name;
}
