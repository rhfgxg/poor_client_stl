#ifndef User_Manager_H
#define User_Manager_H

#include "server_login.grpc.pb.h"	// 登录服务
#include "common.grpc.pb.h" // 公共文件：包含服务类型等
#include "gateway_manager.h" // 网关管理
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

// 登录服务实现类
class UserManager
{
public:
    UserManager(GatewayManager& gateway_manager_);
    ~UserManager();

    void start_thread_pool(int num_threads);    // 启动线程池
    void stop_thread_pool();    // 停止线程池

    // 将数据序列化为 grpc请求后，发送给服务器
    void Handle_login(const std::string account, const std::string password);    // 登录
    void Handle_register(const rpc_server::RegisterReq* req, rpc_server::RegisterRes* res);    // 注册
    void Handle_authenticate(const std::string account, const std::string token);    // 令牌验证

private:
    std::future<void> add_async_task(std::function<void()> task); // 添加异步任务
    void Worker_thread();   // 执行线程的任务

    // 定时任务：

    // 工具函数
    void Save_token(const std::string& account, const std::string& token);  // 保存令牌
private:
    LoggerManager logger_manager;  // 日志管理器

    GatewayManager& gateway_manager; // 网关管理

    std::vector<std::thread> thread_pool;   // 线程池
    std::queue<std::function<void()>> task_queue;    // 任务队列
    std::mutex queue_mutex; // 任务队列互斥锁
    std::condition_variable queue_cv;   // 任务队列条件变量
    bool stop_threads = false;  // 停止线程标志
};

#endif // User_Manager_H
