#ifndef GATEWAY_MANAGER_H
#define GATEWAY_MANAGER_H

#include "common.grpc.pb.h" // 公共文件：包含服务类型等
#include "server_gateway.grpc.pb.h" // 网关服务器
#include "connection_pool.h"    // 连接池（网关服务器）
#include "logger_manager.h"     // 日志管理器

#include <grpcpp/grpcpp.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

// 网络模块：负责与服务器通信，心跳等
class GatewayManager
{
public:
    GatewayManager();
    ~GatewayManager();

    void start_thread_pool(int num_threads);    // 启动线程池
    void stop_thread_pool();    // 停止线程池

    // 转发服务请求
    grpc::Status Request_forward(const google::protobuf::Message* req, google::protobuf::Message* res, rpc_server::ServiceType service_type);

private:
    // 初始化
    void Client_register();    // 客户端注册
    void Get_gateway_pool();    // 获取网关服务器连接池
    std::string get_client_ip();    // 获取客户端ip地址

    // 多线程
    std::future<void> add_async_task(std::function<void()> task); // 添加异步任务
    void Worker_thread();   // 执行线程的任务

    // 定时任务：
    void Send_heartbeat();  // 发送心跳包
    void Update_connection_pool();  // 更新连接池

private:
    std::string client_token;   // 客户端token

    LoggerManager logger_manager;  // 日志管理器

    ConnectionPool gateway_connection_pool;   // 网关服务器连接池
    
    std::vector<std::thread> thread_pool;   // 线程池
    std::queue<std::function<void()>> task_queue;    // 任务队列
    std::mutex queue_mutex; // 任务队列互斥锁
    std::condition_variable queue_cv;   // 任务队列条件变量
    bool stop_threads = false;  // 停止线程标志
};

#endif // GATEWAY_MANAGER_H
