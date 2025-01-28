#ifndef GATEWAY_SERVER_H
#define GATEWAY_SERVER_H

#include "common.grpc.pb.h" // 公共文件：包含服务类型等
#include "server_gateway.grpc.pb.h" // 网关服务器
#include "logger_manager.h"     // 日志管理器
#include "plugin_manager.h"     // 插件管理器
#include "file_plugin.h"        // 文件服务器

#include <grpcpp/grpcpp.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

// 网络模块：负责与服务器通信，心跳等
class GatewayServerImpl final: public rpc_server::GatewayServer::Service
{
public:
    GatewayServerImpl(LoggerManager& logger_manager_);
    ~GatewayServerImpl();

    void start_thread_pool(int num_threads);    // 启动线程池
    void stop_thread_pool();    // 停止线程池

    // 转发服务请求
    grpc::Status Request_forward(grpc::ServerContext* context, const rpc_server::ForwardReq* req, rpc_server::ForwardRes* res);
    // 获取文件服务器地址
    grpc::Status Get_file_server_address(grpc::ServerContext* context, const rpc_server::GetFileServerAddressReq* req, rpc_server::GetFileServerAddressRes* res);
    // 接收客户端心跳
    grpc::Status Client_heartbeat(grpc::ServerContext* context, const rpc_server::ClientHeartbeatReq* req, rpc_server::ClientHeartbeatRes* res);

private:
    // 多线程
    std::future<void> add_async_task(std::function<void()> task); // 添加异步任务
    void Worker_thread();   // 执行线程的任务

    // 处理转发请求
    grpc::Status Forward_to_login_service(const std::string& payload, rpc_server::ForwardRes* response);

    // 定时任务：
    void Send_heartbeat();  // 发送心跳包

private:
    LoggerManager& logger_manager;  // 日志管理器
    PluginManager plugin_manager;   // 插件管理器

    // 网关服务器连接池
    
    std::vector<std::thread> thread_pool;   // 线程池
    std::queue<std::function<void()>> task_queue;    // 任务队列
    std::mutex queue_mutex; // 任务队列互斥锁
    std::condition_variable queue_cv;   // 任务队列条件变量
    bool stop_threads = false;  // 停止线程标志
};

#endif // GATEWAY_SERVER_H
