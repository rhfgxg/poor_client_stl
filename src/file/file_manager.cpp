#include "file_manager.h"

#include <fstream>

FileManager::FileManager(GatewayManager& gateway_manager_):
    logger_manager(),
    gateway_manager(gateway_manager_)
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
grpc::Status FileManager::Upload()
{
    rpc_server::UploadReq req;
    rpc_server::UploadRes res;

    // 通过网关转发，向服务器发送请求
    grpc::Status status = gateway_manager.Request_forward(&req, &res, rpc_server::ServiceType::REQ_FILE_UPLOAD);
    if(status.ok() && res.success())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("File upload success");
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("File upload failed");
    }

}

// 文件下载服务
grpc::Status FileManager::Download()
{
    rpc_server::DownloadReq req;
    rpc_server::DownloadRes res;

    // 通过网关转发，向服务器发送请求
    grpc::Status status = gateway_manager.Request_forward(&req, &res, rpc_server::ServiceType::REQ_FILE_DOWNLOAD);
    if(status.ok() && res.success())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("File download success");
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("File download failed");
    }
}

// 文件删除服务
grpc::Status FileManager::Delete()
{
    rpc_server::DeleteFileReq req;
    rpc_server::DeleteFileRes res;

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
grpc::Status FileManager::ListFiles()
{
    rpc_server::ListFilesReq req;
    rpc_server::ListFilesRes res;

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

/************************************ gRPC服务接口工具函数 **************************************************/

