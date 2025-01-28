#ifndef FILE_SERVER_H
#define FILE_SERVER_H

#include "../../../protobuf/src/common.grpc.pb.h" // 公共文件：包含服务类型等
#include "../../../protobuf/src/server_file.grpc.pb.h"  // 文件服务
#include "logger_manager.h"     // 日志管理器

#include <grpcpp/grpcpp.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

// 网关服务器对外接口
class FileServerImpl final: public rpc_server::FileServer::Service
{
public:
    FileServerImpl(LoggerManager& logger_manager_);
    ~FileServerImpl();

    void start_thread_pool(int num_threads);    // 启动线程池
    void stop_thread_pool();    // 停止线程池

    // 文件上传服务
    grpc::Status Upload(grpc::ServerContext* context, const rpc_server::UploadReq* req, rpc_server::UploadRes* res);
    // 文件下载服务
    grpc::Status Download(grpc::ServerContext* context, const rpc_server::DownloadReq* req, rpc_server::DownloadRes* res);
    // 文件删除服务
    grpc::Status Delete(grpc::ServerContext* context, const rpc_server::DeleteFileReq* req, rpc_server::DeleteFileRes* res);
    // 文件列表服务
    grpc::Status ListFiles(grpc::ServerContext* context, const rpc_server::ListFilesReq* req, rpc_server::ListFilesRes* res);

private:
    // 多线程
    std::future<void> add_async_task(std::function<void()> task); // 添加异步任务
    void Worker_thread();   // 执行线程的任务

private:
    LoggerManager& logger_manager;  // 日志管理器

    std::vector<std::thread> thread_pool;   // 线程池
    std::queue<std::function<void()>> task_queue;    // 任务队列
    std::mutex queue_mutex; // 任务队列互斥锁
    std::condition_variable queue_cv;   // 任务队列条件变量
    bool stop_threads = false;  // 停止线程标志
};


#endif // !FILE_SERVER_H
