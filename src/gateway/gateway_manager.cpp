#include "gateway_manager.h"

#ifdef WIN32
#include <boost/asio.hpp>
#include <iostream>
#elif __linux__
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif

GatewayManager::GatewayManager():
    logger_manager(),
    gateway_connection_pool(10) // 设置网关服务器连接池大小为10
{
    start_thread_pool(10); // 启动线程池

    Client_register(); // 客户端注册

// 定时任务
    // 向服务器获取最新的网关连接池
    std::thread(&GatewayManager::Update_connection_pool, this).detach();
    // 定时向服务器发送心跳包
    std::thread(&GatewayManager::Send_heartbeat, this).detach();

    logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->info("Gateway Service started"); // 启动日志
}

GatewayManager::~GatewayManager()
{
    stop_thread_pool(); // 停止并清空线程池

    // 记录关闭日志
    logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->info("Gateway Service stopped");
    // 停止并清理日志管理器
    logger_manager.cleanup();
}

/*************************************** 多线程工具函数 *****************************************************************/
// 启动线程池
void GatewayManager::start_thread_pool(int num_threads)
{// 相关注释请参考 /src/central/src/central/central_server.cpp/start_thread_pool()
    for(int i = 0; i < num_threads; ++i)
    {
        this->thread_pool.emplace_back(&GatewayManager::Worker_thread, this);   // 创建线程
    }
}

// 停止线程池
void GatewayManager::stop_thread_pool()
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
    std::swap(this->task_queue,empty);
}

// 添加异步任务
std::future<void> GatewayManager::add_async_task(std::function<void()> task)
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
void GatewayManager::Worker_thread()
{// 相关注释请参考 /src/central/src/central/central_server.cpp/worker_thread()
    while(true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);

            this->queue_cv.wait(lock,[this] {
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
// 定时任务：发送心跳包
void GatewayManager::Send_heartbeat()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10)); // 每10秒发送一次心跳包

        rpc_server::ClientHeartbeatReq req;
        rpc_server::ClientHeartbeatRes res;
        grpc::ClientContext context;

        req.set_client_token(this->client_token); // 设置客户端token
        req.set_address("127.0.0.1"); // 设置客户端ip

        // 获取连接池中的连接
        auto channel = this->gateway_connection_pool.get_connection();
        auto gateway_stub = rpc_server::GatewayServer::NewStub(channel);

        grpc::Status status = gateway_stub->Client_heartbeat(&context, req, &res);

        if(status.ok() && res.success())
        {
            logger_manager.getLogger(rpc_server::LogCategory::HEARTBEAT)->info("Heartbeat sent successfully.");
        }
        else
        {
            logger_manager.getLogger(rpc_server::LogCategory::HEARTBEAT)->error("Failed to send heartbeat.");
        }

        this->gateway_connection_pool.release_connection(channel); // 释放连接
    }
}

// 定时任务：更新连接池
void GatewayManager::Update_connection_pool()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::minutes(5)); // 每5分钟更新一次连接池
        Get_gateway_pool();
    }
}

/**************************************** grpc服务接口定义 **************************************************************************/
// 服务转发接口
grpc::Status GatewayManager::Request_forward(const google::protobuf::Message* req, google::protobuf::Message* res, rpc_server::ServiceType service_type)
{
    /*
    参数：原始请求包，服务类型
    函数功能：将原始请求包 包装为网关服务器的转发请求，然后发送给网关服务器转发给对应服务器
    */

    // 包装为转发请求
    rpc_server::ForwardReq forward_req;
    rpc_server::ForwardRes forward_res;
    grpc::ClientContext client_context; // 包含 RPC 调用的元数据和其他信息

    forward_req.set_service_type(service_type); // 设置服务类型
    forward_req.set_payload(req->SerializeAsString());  // 序列化 原始请求 为 payload

    // 获取连接池中的连接
    auto channel = this->gateway_connection_pool.get_connection();
    auto gateway_stub = rpc_server::GatewayServer::NewStub(channel);

    // 向网关服务器发送请求
    grpc::Status status = gateway_stub->Request_forward(&client_context, forward_req, &forward_res);

    if(status.ok() && forward_res.success())
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->info("Forward sent successfully.");

        // 解析响应包，得到原始请求的响应
        if(!res->ParseFromString(forward_res.response()))
        {
            logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("Failed to parse response.");
            return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to parse response");
        }
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::APPLICATION_ACTIVITY)->error("Failed to send Forward.");
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to send Forward");
    }

    this->gateway_connection_pool.release_connection(channel); // 释放连接

    return grpc::Status::OK;
}

/**************************************** grpc服务接口工具函数 **************************************************************************/

/******************************************** 其他工具函数 ***********************************************/
// 客户端注册
void GatewayManager::Client_register()
{
    /* 客户端链接服务器
    * 上传客户端ip地址
    * 获取并初始化网关服务器连接池（使用配置文件中的默认网关服务器地址）
    * 获取客户端token
    */
    std::string client_ip = this->get_client_ip(); // 获取客户端ip地址

    // 客户端
    grpc::ClientContext context;
    // 请求
    rpc_server::ClientRegisterReq req;
    req.set_address(client_ip);
    // 响应
    rpc_server::ClientRegisterRes res;

    auto gateway_stub = rpc_server::GatewayServer::NewStub(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));

    grpc::Status status = gateway_stub->Client_register(&context, req, &res);

    if(status.ok() && res.success())
    {
        for(const rpc_server::GatewayConnectInfo& conn_info : res.connect_info())
        {
            gateway_connection_pool.add_connection(conn_info.address(), std::to_string(conn_info.port()));
        }
        logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->info("Client registered successfully");

        this->client_token = res.client_token();
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->error("Client registration failed");
    }
}

// 获取网关服务器连接池
void GatewayManager::Get_gateway_pool()
{
    /* 获取并初始化网关服务器连接池
    * 使用连接池中信息
    * 上传客户端token
    * 获取网关服务器连接池
    */
    // 客户端
    grpc::ClientContext context;
    // 请求
    rpc_server::GetGatewayPoolReq req;
    req.set_client_token(this->client_token);
    // 响应
    rpc_server::GetGatewayPoolRes res;

    // 获取连接池中的连接
    auto channel = this->gateway_connection_pool.get_connection();
    auto gateway_stub = rpc_server::GatewayServer::NewStub(channel);

    grpc::Status status = gateway_stub->Get_gateway_pool(&context, req, &res);

    if(status.ok() && res.success())
    {
        for(const rpc_server::GatewayConnectInfo& conn_info : res.connect_info())
        {
            gateway_connection_pool.add_connection(conn_info.address(), std::to_string(conn_info.port()));
        }
        logger_manager.getLogger(rpc_server::LogCategory::CONNECTION_POOL)->info("Gateway server updated connection pools successfully");
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::CONNECTION_POOL)->error("Failed to get connection pools information");
    }
}

// 获取客户端ip地址
std::string GatewayManager::get_client_ip()
{
    /*获取客户端ip地址
    * 返回值：客户端ip地址
    * 为了实现跨平台，使用了不同的方法
    * windwos下使用boost库，linux下使用系统调用
    */
#ifdef WIN32
    try
    {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);

        while(it != boost::asio::ip::tcp::resolver::iterator())
        {
            boost::asio::ip::tcp::endpoint endpoint = *it++;
            if(endpoint.address().is_v4())
            {
                return endpoint.address().to_string();
            }
        }
    } catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return "";

#elif __linux__
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    std::string ip_address;

    if(getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return "";
    }

    for(ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if(ifa->ifa_addr == nullptr)
            continue;

        int family = ifa->ifa_addr->sa_family;

        if(family == AF_INET) {
            if(getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST) == 0) {
                ip_address = host;
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return ip_address;

#endif // WIN32

    
}
