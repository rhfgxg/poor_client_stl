#include "user_manager.h"

#include <fstream>
#include <iostream>

UserManager::UserManager():
    logger_manager(),
    gateway_manager()
{
    logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->info("Login service started"); // 启动日志

    // 启动定时任务
}

UserManager::~UserManager()
{
    stop_thread_pool(); // 停止并清空线程池

    // 记录关闭日志
    logger_manager.getLogger(rpc_server::LogCategory::STARTUP_SHUTDOWN)->info("LoginServer stopped");
    // 停止并清理日志管理器
    logger_manager.cleanup();
}

/************************************ 线程池工具函数 ****************************************************/
// 启动线程池
void UserManager::start_thread_pool(int num_threads)
{// 相关注释请参考 /src/central/src/central/central_server.cpp/start_thread_pool()
    for(int i = 0; i < num_threads; ++i)
    {
        this->thread_pool.emplace_back(&UserManager::Worker_thread, this);   // 创建线程
    }
}

// 停止线程池
void UserManager::stop_thread_pool()
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
std::future<void> UserManager::add_async_task(std::function<void()> task)
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
void UserManager::Worker_thread()
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

/************************************ 定时任务 ********************************************************/

/************************************ gRPC服务接口工具函数 **************************************************/
// 登录服务
void UserManager::Handle_login(const std::string account, const std::string password)
{
    rpc_server::LoginReq login_req;	// 登录请求
    rpc_server::LoginRes login_res; // 登录响应
    login_req.set_account(account);    // 设置用户账号
    login_req.set_password(password);   // 设置密码

    // 通过网关转发，向服务器发送请求
    grpc::Status status = gateway_manager.Request_forward(&login_req, &login_res, rpc_server::ServiceType::REQ_LOGIN);

    if(status.ok() && login_res.success())
    {
        logger_manager.getLogger(rpc_server::LogCategory::HEARTBEAT)->info("Login success");
        this->Save_token(account, login_res.token());    // 保存/更新令牌
    }
    else
    {
        logger_manager.getLogger(rpc_server::LogCategory::HEARTBEAT)->error("Login failed");
    }
}

// 注册服务
void UserManager::Handle_register(const rpc_server::RegisterReq* req,rpc_server::RegisterRes* res)    // 注册
{
}

// 令牌验证服务
void UserManager::Handle_authenticate(const std::string account, const std::string token)
{
    // 验证请求
    rpc_server::AuthenticateReq authenticate_req;
    authenticate_req.set_account(account);    // 设置用户账号
    authenticate_req.set_token(token);   // 设置token

    rpc_server::AuthenticateRes authenticate_res;   // 响应

    // 调用网关转发，向服务器发送请求
    grpc::Status status = gateway_manager.Request_forward(&authenticate_req, &authenticate_res, rpc_server::ServiceType::REQ_AUTHENTICATE);

    if(status.ok() && authenticate_res.success())
    {
        std::cout << "token validated" << std::endl;
    }
    else
    {
        std::cout << "token failed" << std::endl;
    }

}


/******************************************** 其他工具函数 ***********************************************/
// 保存令牌
void UserManager::Save_token(const std::string& account, const std::string& token)
{
    std::ifstream config_file_in("./config/local.ini"); // 打开配置文件
    std::string content;    // 配置文件内容
    bool found = false; // 是否找到账号
    bool token_field_found = false; // 是否找到 token 字段

    if(config_file_in.is_open())
    {
        std::string line;
        while(std::getline(config_file_in, line))  // 逐行读取
        {
            if(line.find("token=") == 0)   // 找到 token 字段
            {
                token_field_found = true;
                size_t pos = line.find(account + ":");    // 查找账号
                if(pos != std::string::npos)
                {
                    size_t end_pos = line.find(";", pos);    // 查找令牌
                    line.replace(pos, end_pos - pos, account + ":" + token);    // 更新令牌
                    found = true;
                }
                else   // token字段存在，账号不存在
                {
                    line += account + ":" + token + ";"; // 添加账号和令牌
                }
                content += line + "\n";
            }
            else
            {
                content += line + "\n";
            }
        }
        config_file_in.close();
    }

    if(!token_field_found) // 没有找到 token 字段
    {
        content += "token=" + account + ":" + token + ";\n";
    }

    std::ofstream config_file_out("./config/local.ini");
    if (config_file_out.is_open())
    {
        config_file_out << content;
        config_file_out.close();
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
    }
}
