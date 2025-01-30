#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <grpcpp/grpcpp.h>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <condition_variable>

// 连接池类，管理所有连接
class ConnectionPool
{
public:
    ConnectionPool(size_t pool_size);   // 参数：连接池大小
    ~ConnectionPool();

    // 获取连接
    std::shared_ptr<grpc::Channel> get_connection();
    // 释放连接
    void release_connection(std::shared_ptr<grpc::Channel> connection);
    // 更新连接池中的连接
    void update_connections(const std::string& server_address, const std::string& server_port);

    void add_connection(const std::string& server_address, const std::string& server_port);

private:
    // 创建新的连接
    std::shared_ptr<grpc::Channel> New_connection(const std::string& server_address, const std::string& server_port);

private:
    size_t pool_size;  // 连接池大小
    std::queue<std::shared_ptr<grpc::Channel>> connection_pool; // 连接池
    std::pair<std::string, std::string> server_info; // 当前服务器信息
    std::mutex pool_mutex; // 连接池互斥锁
    std::condition_variable pool_cv; // 连接池条件变量,直到连接池中有可用的连接
};

#endif // CONNECTION_POOL_H
