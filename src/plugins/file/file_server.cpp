#include "file_server.h"

FileServerImpl::FileServerImpl(LoggerManager& logger_manager_):
    logger_manager(logger_manager_)
{
    // 启动定时任务
    // 记录启动日志
    logger_manager.getLogger(LogCategory::STARTUP_SHUTDOWN)->info("FileServer started");
}

FileServerImpl::~FileServerImpl()
{
    stop_thread_pool(); // 停止并清空线程池

    // 记录关闭日志
    logger_manager.getLogger(LogCategory::STARTUP_SHUTDOWN)->info("FileServer stopped");
    // 停止并清理日志管理器
    logger_manager.cleanup();
}

/*************************************** 多线程工具函数 *****************************************************************/
// 启动线程池
void FileServerImpl::start_thread_pool(int num_threads)
{// 相关注释请参考 /src/central/src/central/central_server.cpp/start_thread_pool()
    for(int i = 0; i < num_threads; ++i)
    {
        this->thread_pool.emplace_back(&FileServerImpl::Worker_thread, this);   // 创建线程
    }
}

// 停止线程池
void FileServerImpl::stop_thread_pool()
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
std::future<void> FileServerImpl::add_async_task(std::function<void()> task)
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
void FileServerImpl::Worker_thread()
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

/******************************************* 定时任务 *****************************************************/


/**************************************** grpc服务接口定义 **************************************************************************/
// 服务转发接口
grpc::Status FileServerImpl::Upload(grpc::ServerContext* context, const rpc_server::UploadReq* req, rpc_server::UploadRes* res)
{
    auto task_future = this->add_async_task([this, req, res] {
        // this->Handle_create(req, res);   // 执行函数
    });

    // 等待任务完成
    task_future.get();

    return grpc::Status::OK;
}

// 文件下载服务
grpc::Status FileServerImpl::Download(grpc::ServerContext* context, const rpc_server::DownloadReq* req, rpc_server::DownloadRes* res)
{
    return grpc::Status::OK;
}

// 文件删除服务
grpc::Status FileServerImpl::Delete(grpc::ServerContext* context, const rpc_server::DeleteFileReq* req, rpc_server::DeleteFileRes* res)
{
    return grpc::Status::OK;
}

// 文件列表服务
grpc::Status FileServerImpl::ListFiles(grpc::ServerContext* context, const rpc_server::ListFilesReq* req, rpc_server::ListFilesRes* res)
{
    return grpc::Status::OK;
}

/**************************************** grpc服务接口工具函数 **************************************************************************/

/******************************************** 其他工具函数 ***********************************************/
