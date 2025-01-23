#ifndef LOGIN_SERVER_H
#define LOGIN_SERVER_H

#include "server_login.grpc.pb.h"	// 登录服务
#include "logger_manager.h"     // 日志管理器
#include "plugin_manager.h"     // 插件管理器

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
class LoginServerImpl final : public rpc_server::LoginServer::Service
{
public:
    LoginServerImpl(LoggerManager& logger_manager_);	// 构造函数
    ~LoginServerImpl();	// 析构函数

    void start_thread_pool(int num_threads);    // 启动线程池
    void stop_thread_pool();    // 停止线程池

    grpc::Status Login(grpc::ServerContext* context, const rpc_server::LoginReq* req, rpc_server::LoginRes* res) override;  // 登录
    grpc::Status Register(grpc::ServerContext* context, const rpc_server::RegisterReq* req, rpc_server::RegisterRes* res) override;   // 注册
    grpc::Status Authenticate(grpc::ServerContext* context, const rpc_server::AuthenticateReq* req, rpc_server::AuthenticateRes* res) override;   // 令牌验证

private:
    std::future<void> add_async_task(std::function<void()> task); // 添加异步任务
    void Worker_thread();   // 执行线程的任务

    void Handle_login(const rpc_server::LoginReq* req,rpc_server::LoginRes* res);    // 登录
    void Handle_register(const rpc_server::RegisterReq* req,rpc_server::RegisterRes* res);    // 注册
    void Handle_authenticate(const rpc_server::AuthenticateReq* req,rpc_server::AuthenticateRes* res);    // 令牌验证

    // 定时任务：
    void Send_heartbeat();  // 发送心跳包

    // 工具函数

private:
    LoggerManager& logger_manager;  // 日志管理器
    PluginManager plugin_manager;   // 插件管理器

    std::vector<std::thread> thread_pool;   // 线程池
    std::queue<std::function<void()>> task_queue;    // 任务队列
    std::mutex queue_mutex; // 任务队列互斥锁
    std::condition_variable queue_cv;   // 任务队列条件变量
    bool stop_threads = false;  // 停止线程标志
};

#endif // LOGIN_SERVER_H
