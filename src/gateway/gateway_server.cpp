#include "gateway_server.h"

GatewayServerImpl::GatewayServerImpl(LoggerManager& logger_manager_):
    logger_manager(logger_manager_)
{
    // 打开插件
    plugin_manager.LoadPlugin("./plugins/file_plugin.dll");

    IPlugin* plugin = plugin_manager.GetPlugin(0); // 获取第一个插件
    if(plugin)
    {
        plugin->Initialize();
        plugin->Execute();

        // 获取实际功能接口
        FilePlugin* file_plugin = dynamic_cast<FilePlugin*>(plugin);
        if(file_plugin) {
            FileServerImpl* file_server = file_plugin->GetFileServer();
            // 现在可以调用 file_server 的实际功能接口
            // 例如，调用 file_server 的 Upload 方法
            // file_server->Upload(...);
        }
    }

    // 定时向中心服务器发送心跳包
    std::thread(&GatewayServerImpl::Send_heartbeat, this).detach();
}

GatewayServerImpl::~GatewayServerImpl()
{
    stop_thread_pool(); // 停止并清空线程池

    // 卸载所有插件
    plugin_manager.UnloadPlugins();

    // 记录关闭日志
    logger_manager.getLogger(LogCategory::STARTUP_SHUTDOWN)->info("GatewayServer stopped");
    // 停止并清理日志管理器
    logger_manager.cleanup();
}

/*************************************** 多线程工具函数 *****************************************************************/
// 启动线程池
void GatewayServerImpl::start_thread_pool(int num_threads)
{// 相关注释请参考 /src/central/src/central/central_server.cpp/start_thread_pool()
    for(int i = 0; i < num_threads; ++i)
    {
        this->thread_pool.emplace_back(&GatewayServerImpl::Worker_thread, this);   // 创建线程
    }
}

// 停止线程池
void GatewayServerImpl::stop_thread_pool()
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
std::future<void> GatewayServerImpl::add_async_task(std::function<void()> task)
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
void GatewayServerImpl::Worker_thread()
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
void GatewayServerImpl::Send_heartbeat()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10)); // 每10秒发送一次心跳包

        rpc_server::HeartbeatReq request;
        rpc_server::HeartbeatRes response;
        grpc::ClientContext context;

        request.set_server_type(rpc_server::ServerType::GATEWAY);
        request.set_address("127.0.0.1"); // 设置服务器ip
        request.set_port("50051"); // 设置服务器端口

        grpc::Status status = central_stub->Heartbeat(&context,request,&response);

        if(status.ok() && response.success())
        {
            logger_manager.getLogger(LogCategory::HEARTBEAT)->info("Heartbeat sent successfully.");
        }
        else
        {
            logger_manager.getLogger(LogCategory::HEARTBEAT)->error("Failed to send heartbeat.");
        }
    }
}

/**************************************** grpc服务接口定义 **************************************************************************/
// 服务转发接口
grpc::Status GatewayServerImpl::Request_forward(grpc::ServerContext* context, const rpc_server::ForwardReq* req, rpc_server::ForwardRes* res)
{
    auto task_future = this->add_async_task([this,req,res] {
        switch(req->service_type()) // 根据请求的服务类型进行转发
        {
        case rpc_server::ServiceType::REQ_LOGIN: // 用户登录请求
        {
            Forward_to_login_service(req->payload(), res);  // 解析负载，并转发到登录服务
            break;
        }
        default:    // 未知服务类型
        {
            res->set_success(false);
            break;
        }
        }
    });

    // 等待任务完成
    task_future.get();

    return grpc::Status::OK;
}

// 获取文件服务器地址
grpc::Status GatewayServerImpl::Get_file_server_address(grpc::ServerContext* context, const rpc_server::GetFileServerAddressReq* req, rpc_server::GetFileServerAddressRes* res)
{
    return grpc::Status::OK;
}

// 接收客户端心跳
grpc::Status GatewayServerImpl::Client_heartbeat(grpc::ServerContext* context, const rpc_server::ClientHeartbeatReq* req, rpc_server::ClientHeartbeatRes* res)
{
    return grpc::Status::OK;
}

/**************************************** grpc服务接口工具函数 **************************************************************************/
// Login 方法，处理登录请求
grpc::Status GatewayServerImpl::Forward_to_login_service(const std::string& payload, rpc_server::ForwardRes* response)
{
    rpc_server::LoginReq login_req;  // 创建登录请求对象
    bool req_out = login_req.ParseFromString(payload); // 将负载解析为登录请求对象

    if(!req_out) // 如果解析失败
    {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,"Failed to parse LoginRequest");
    }

    // 构造响应
    rpc_server::LoginRes login_response;
    grpc::ClientContext context;

    // 获取连接池中的连接
    auto channel = this->login_connection_pool.get_connection(rpc_server::ServerType::LOGIN);
    auto login_stub = rpc_server::LoginServer::NewStub(channel);

    grpc::Status status = login_stub->Login(&context, login_req, &login_response);

    if(!status.ok()) // 如果调用失败
    {
        return status;
    }

    bool response_out = login_response.SerializeToString(response->mutable_response()); // 将登录响应序列化为转发响应
    if(!response_out) // 如果序列化失败
    {
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to serialize LoginResponse");
    }

    if(login_response.success()) // 如果登录成功
    {
        response->set_success(true);    // 设置响应对象 response 的 success 字段为 true
    }
    else
    {
        response->set_success(false);
    }

    return grpc::Status::OK;
}

/******************************************** 其他工具函数 ***********************************************/
