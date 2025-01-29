#include "gateway_server.h"

GatewayServerImpl::GatewayServerImpl(LoggerManager& logger_manager_):
    logger_manager(logger_manager_),
    gateway_connection_pool(10) // 设置网关服务器连接池大小为10
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
            //FileServerImpl* file_server = file_plugin->Get_file_server();
            // 现在可以调用 file_server 的实际功能接口
            // 例如，调用 file_server 的 Upload 方法
            // file_server->Upload(...);
        }
    }

    // 定时向服务器获取最新的连接池状态
    //std::thread(&GatewayServerImpl::Update_connection_pool, this).detach();
    // 定时向服务器发送心跳包
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

        rpc_server::ClientHeartbeatReq req;
        rpc_server::ClientHeartbeatRes res;
        grpc::ClientContext context;

        req.set_account("13411806653");  // 设置账户
        req.set_token(""); // 设置token
        req.set_address("127.0.0.1"); // 设置服务器ip

        // 获取连接池中的连接
        auto channel = this->gateway_connection_pool.get_connection();
        auto gateway_stub = rpc_server::GatewayServer::NewStub(channel);

        grpc::Status status = gateway_stub->Client_heartbeat(&context, req, &res);

        if(status.ok() && res.success())
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
    auto task_future = this->add_async_task([this, req, res] {
        Forward_request_service(req, res);
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

/**************************************** grpc服务接口工具函数 **************************************************************************/
// 处理转发请求：将所有服务统一转发到服务器
grpc::Status GatewayServerImpl::Forward_request_service(const rpc_server::ForwardReq* req, rpc_server::ForwardRes* res)
{
    // 获取连接池中的连接
    auto channel = this->gateway_connection_pool.get_connection();
    auto gateway_stub = rpc_server::GatewayServer::NewStub(channel);
    grpc::ClientContext context;

    grpc::Status status = gateway_stub->Request_forward(&context, *req, res);

    if(status.ok() && (*res).success())
    {
        std::cout << "Register successful" << std::endl;
    }
    else
    {
        std::cout << "Register failed" << std::endl;
    }

    return grpc::Status::OK;
}

/******************************************** 其他工具函数 ***********************************************/
