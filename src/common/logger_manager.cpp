#include "logger_manager.h"

void LoggerManager::initialize()
{
    // 创建日志文件夹
    this->log_directory = "./logs/";
    std::filesystem::create_directories(this->log_directory); 

    // 创建控制台日志器
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");

    // 创建不同分类的文件日志器
    auto create_file_logger = [&](rpc_server::LogCategory category, const std::string& log_type) {
        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_directory + "/" + log_type + "/log.log", 0, 0); // 按日期滚动
        file_sink->set_level(spdlog::level::debug);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");

        spdlog::sinks_init_list sink_list = {console_sink,file_sink};
        auto logger = std::make_shared<spdlog::logger>(std::to_string(static_cast<int>(category)), sink_list.begin(), sink_list.end());
        logger->set_level(spdlog::level::debug);
        this->loggers[category] = logger;
    };

    // 创建所有分类的日志器
    create_file_logger(rpc_server::LogCategory::STARTUP_SHUTDOWN, "startup_shutdown");
    create_file_logger(rpc_server::LogCategory::APPLICATION_ACTIVITY, "application_activity");
    create_file_logger(rpc_server::LogCategory::CONNECTION_POOL, "connection_pool");
    create_file_logger(rpc_server::LogCategory::SYSTEM_MONITORING, "system_monitoring");
    create_file_logger(rpc_server::LogCategory::HEARTBEAT,"heartbeat");
    create_file_logger(rpc_server::LogCategory::SECURITY, "security");
    create_file_logger(rpc_server::LogCategory::CONFIGURATION_CHANGES, "configuration_changes");
    create_file_logger(rpc_server::LogCategory::DATABASE_OPERATIONS,"database_operations");
    create_file_logger(rpc_server::LogCategory::USER_ACTIVITY,"user_activity");
    create_file_logger(rpc_server::LogCategory::NETWORK,"network");

    // 设置默认日志器
    spdlog::set_default_logger(this->loggers[rpc_server::LogCategory::APPLICATION_ACTIVITY]);
    spdlog::set_level(spdlog::level::debug); // 设置全局日志级别
    spdlog::flush_on(spdlog::level::info); // 设置日志刷新级别
}

// 清理日志器
void LoggerManager::cleanup()
{
    this->loggers.clear();
}

// 获取日志器
std::shared_ptr<spdlog::logger> LoggerManager::getLogger(rpc_server::LogCategory category)
{
    auto it = this->loggers.find(category);
    if(it != this->loggers.end())
    {
        return it->second;
    }
    else
    {
        // 如果日志器不存在，返回默认日志器
        return spdlog::default_logger();
    }
}
