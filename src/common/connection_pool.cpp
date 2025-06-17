#include "connection_pool.h"

ConnectionPool::ConnectionPool(size_t pool_size_):
    pool_size(pool_size_)
{

}

ConnectionPool::~ConnectionPool()
{
    std::lock_guard<std::mutex> lock(pool_mutex);   // 加锁
    while(!connection_pool.empty())    // 如果连接池不为空
    {
        connection_pool.pop();  // 弹出连接
    }
}

// 获取链接
std::shared_ptr<grpc::Channel> ConnectionPool::get_connection()
{
    std::unique_lock<std::mutex> lock(pool_mutex);   // 加锁
    this->pool_cv.wait(lock, [this] {
        return !this->connection_pool.empty();
    }); // 等待直到连接池不为空

    auto connection = this->connection_pool.front();    // 获取连接
    this->connection_pool.pop();    // 弹出连接
    return connection;
}

// 释放链接
void ConnectionPool::release_connection(std::shared_ptr<grpc::Channel> connection)
{
    std::lock_guard<std::mutex> lock(this->pool_mutex);   // 加锁
    this->connection_pool.push(connection);  // 加入连接池
    this->pool_cv.notify_one(); // 通知等待的线程
}

// 更新连接池中的连接
void ConnectionPool::update_connections(const std::string& server_address, const std::string& server_port)
{
    std::lock_guard<std::mutex> lock(this->pool_mutex);   // 加锁

    // 更新服务器信息
    this->server_info = {server_address, server_port};

    // 清空当前连接池
    while(!this->connection_pool.empty())
    {
        this->connection_pool.pop();
    }

    // 添加新连接到连接池
    for(size_t i = 0; i < pool_size; ++i)
    {
        auto channel = New_connection(server_address, server_port);
        this->connection_pool.push(channel);
    }
}

// 添加链接
void ConnectionPool::add_connection(const std::string& server_address, const std::string& server_port)
{
    std::lock_guard<std::mutex> lock(this->pool_mutex);   // 加锁
    this->server_info = {server_address, server_port};    // 更新服务器信息
    for(size_t i = 0; i < pool_size; ++i)
    {
        auto channel = New_connection(server_address, server_port);
        this->connection_pool.push(channel);
    }
}

// 创建新的连接
std::shared_ptr<grpc::Channel> ConnectionPool::New_connection(const std::string& server_address, const std::string& server_port)
{
    return grpc::CreateChannel(server_address + ":" + server_port, grpc::InsecureChannelCredentials());
}
