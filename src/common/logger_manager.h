#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H

#include "common.grpc.pb.h" // 公共文件：包含日志类型，日志等级等

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <ctime>

// 日志管理器类，管理所有日志
class LoggerManager
{
public:
    // 初始化日志器
    void initialize();
    // 清理日志器
    void cleanup();

    // 获取日志器
    std::shared_ptr<spdlog::logger> getLogger(rpc_server::LogCategory category);

private:

private:
    std::unordered_map<rpc_server::LogCategory, std::shared_ptr<spdlog::logger>> loggers;   // 日志器容器
    std::string log_directory;  // 日志文件夹
};

#endif // LOGGER_MANAGER_H

