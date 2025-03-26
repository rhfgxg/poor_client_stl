#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "server_file.grpc.pb.h"	// 登录服务
#include "gateway_manager.h" // 网关管理
#include "user_manager.h"   // 用户管理
#include "logger_manager.h"     // 日志管理器

#include <grpcpp/grpcpp.h>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

// 文件管理器
class FileManager
{
public:
    FileManager(GatewayManager& gateway_manager_, UserManager& user_manager_);
    ~FileManager();

    void start_thread_pool(int num_threads);    // 启动线程池
    void stop_thread_pool();    // 停止线程池

    // 文件上传服务
    void Upload(std::string file_name_);
    // 文件下载服务
    void Download();
    // 文件删除服务
    void Delete();
    // 获取文件列表服务
    void ListFiles();

private:
    std::future<void> add_async_task(std::function<void()> task); // 添加异步任务
    void Worker_thread();   // 执行线程的任务

private:
    LoggerManager logger_manager;  // 日志管理器

    GatewayManager& gateway_manager; // 网关管理
    UserManager& user_manager;   // 用户管理

    std::vector<std::thread> thread_pool;   // 线程池
    std::queue<std::function<void()>> task_queue;    // 任务队列
    std::mutex queue_mutex; // 任务队列互斥锁
    std::condition_variable queue_cv;   // 任务队列条件变量
    bool stop_threads = false;  // 停止线程标志
};

#endif // FILE_MANAGER_H
