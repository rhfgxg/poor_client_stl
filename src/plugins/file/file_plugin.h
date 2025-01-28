#ifndef FILE_PLUGIN_H
#define FILE_PLUGIN_H

#include "plugin_interface.h"   // 插件接口类
#include "file_server.h"    // 文件接口实现类

class FilePlugin: public IPlugin
{
public:
    FilePlugin();
    ~FilePlugin();

    void Initialize() override;
    void Execute() override;
    void Shutdown() override;

    FileServerImpl* GetFileServer(); // 新增方法，获取实际功能接口
private:
    FileServerImpl* file_server;    // 文件接口实现类
};

extern "C" IPlugin* CreatePlugin(); // 创建插件
extern "C" void DestroyPlugin(IPlugin* plugin); // 销毁插件

#endif // FILE_PLUGIN_H
