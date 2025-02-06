#include "plugin_manager.h"     // 插件管理器
#include "file_plugin.h"   // 文件模块插件
#include "user_manager.h"
#include "gateway_manager.h"

#include <fstream>
#include <iostream>

void config();  // 读取客户端配置文件，初始化客户端配置

// 主模块 main函数
int main()
{
    config();   // 读取客户端配置文件，初始化客户端配置

    std::string account = "3056078308";
    std::string password = "159357";

    //GatewayManager gateway_manager; // 网关管理器，单例

    //UserManager user_manager(gateway_manager);  // 用户管理器
    //user_manager.Handle_login(account, password);   // 登录

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

void config()
{
    // 读取配置文件
    std::ifstream config_file_in("./config/local.ini"); // 打开配置文件
    std::string content;    // 配置文件内容
    // 初始化客户端配置

    // 如果配置文件不存在，创建默认配置文件
    if(config_file_in.is_open())
    {
        // 读取配置文件内容
        std::string line;
        while(std::getline(config_file_in, line))
        {
            content += line;
        }
        config_file_in.close();
    }
    else
    {
        // 创建默认配置文件
        std::ofstream config_file_out("./config/local.ini");    // 打开配置文件
        if(config_file_out.is_open())
        {
            // 默认配置
            content = "log_dir=./logs\n";   // 日志目录

            // 写入
            config_file_out << content;
            config_file_out.close();
        }
        else
        {
            std::cout << "Failed to create config file" << std::endl;
        }

    }
}
