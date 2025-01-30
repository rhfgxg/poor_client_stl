#include "plugin_manager.h"     // 插件管理器
#include  "file_plugin.h"   // 文件模块插件
#include "user_manager.h"

// 主模块 main函数
int main()
{
    std::string account = "13411806653";
    std::string password = "159357";

    UserManager user_manager;
    user_manager.start_thread_pool(10);

    user_manager.Handle_login(account, password);

    return 0;
}

void test_plugin()
{
    PluginManager plugin_manager;   // 插件管理器


    // 打开插件
    plugin_manager.LoadPlugin("./plugins/file_plugin.dll");

    IPlugin* plugin = plugin_manager.GetPlugin(0); // 获取第一个插件
    if(plugin)
    {
        plugin->Initialize();
        plugin->Execute();

        // 获取实际功能接口
        FilePlugin* file_plugin = dynamic_cast<FilePlugin*>(plugin);
        if(file_plugin)
        {
            //FileServerImpl* file_server = file_plugin->Get_file_server();
            // 现在可以调用 file_server 的实际功能接口
            // 例如，调用 file_server 的 Upload 方法
            // file_server->Upload(...);
        }
    }

    // 卸载所有插件
    plugin_manager.UnloadPlugins();
}
