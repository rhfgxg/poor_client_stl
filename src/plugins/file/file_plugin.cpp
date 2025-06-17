#include "file_plugin.h"
#include "logger_manager.h"

FilePlugin::FilePlugin()
{
    LoggerManager logger_manager;   // 日志管理器
    file_server = new FileServerImpl(logger_manager);   // 创建文件服务器
}

FilePlugin::~FilePlugin()
{
    delete file_server;
}

// 创建插件实例
void FilePlugin::Initialize()
{
    // 初始化文件服务器
    file_server->start_thread_pool(4); // 启动线程池
}

// 执行插件服务
void FilePlugin::Execute()
{
    // 执行文件服务器的功能
    // 例如，处理文件上传、下载等请求
}

// 关闭插件
void FilePlugin::Shutdown()
{
    // 关闭文件服务器
    file_server->stop_thread_pool(); // 停止线程池
}

// 获取文件服务器对象
FileServerImpl* FilePlugin::Get_file_server()
{
    return file_server; // 返回文件服务器对象
}

extern "C"  // 按照c语言的方式来处理这两个函数的名称：不进行名称修饰
{
#ifdef WIN32
    // 获取插件对象
    __declspec(dllexport) IPlugin* CreatePlugin()
    {
        return new FilePlugin();    // 创建插件对象，并返回地址
    }

    // 释放插件对象
    __declspec(dllexport) void DestroyPlugin(IPlugin* plugin)
    {
        delete plugin;
    }
#endif // WIN32
}
