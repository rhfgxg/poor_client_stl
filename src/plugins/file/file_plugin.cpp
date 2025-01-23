#include "file_plugin.h"
#include "logger_manager.h"

FilePlugin::FilePlugin() {
    // 初始化文件服务器
    LoggerManager logger_manager;
    file_server = new FileServerImpl(logger_manager);
}

FilePlugin::~FilePlugin() {
    delete file_server;
}

void FilePlugin::Initialize() {
    // 初始化文件服务器
    file_server->start_thread_pool(4); // 启动线程池
}

void FilePlugin::Execute() {
    // 执行文件服务器的功能
    // 例如，处理文件上传、下载等请求
}

void FilePlugin::Shutdown() {
    // 关闭文件服务器
    file_server->stop_thread_pool(); // 停止线程池
}

extern "C" IPlugin* CreatePlugin() {
    return new FilePlugin();
}

extern "C" void DestroyPlugin(IPlugin* plugin) {
    delete plugin;
}
