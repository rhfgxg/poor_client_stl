#include "plugin_manager.h"     // 插件管理器
#include "file_plugin.h"   // 文件模块插件
#include "user_manager.h"   // 用户管理器
#include "gateway_manager.h"    // 网关管理器
#include "file_manager.h"   // 文件管理器

#include <string>
#include <fstream>
#include <iostream>

#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

void Test_plugin();   // 插件测试
void Config();  // 读取客户端配置文件，初始化客户端配置
std::string Sha256(const std::string& str); // SHA256哈希加密函数（生成64位16进制数）（HEX编码）

// 主模块 main函数
int main()
{
    Config();   // 读取客户端配置文件，初始化客户端配置

    const std::string user_name = "lhw";
    const std::string account = "3056078308";
    const std::string password = "159357";
    const std::string email = "3056078308@qq.com";
    const std::string phone = "";
    const std::string id_number = "";

    std::string hashed_password = Sha256(password); // 生成64位 16进制 哈希密码

    GatewayManager gateway_manager; // 网关管理器，单例

    UserManager user_manager(gateway_manager);  // 用户管理器
    //user_manager.Register(user_name, hashed_password, email, phone, id_number);    // 注册
    //user_manager.Login(account, hashed_password);   // 登录
    //user_manager.Logout(account);   // 登出
    //user_manager.Change_password(hashed_password, sha256("123456"));    // 修改密码

    FileManager file_manager(gateway_manager, user_manager);    // 文件管理器
    file_manager.Upload("C:/Users/lhw/Pictures/DCIM/花鸟鱼虫/_DSC0984.ARW");  // 文件上传服务
    //file_manager.Download("_DSC0984.ARW");  // 文件下载服务
    //file_manager.Delete("test.txt");    // 文件删除服务
    //file_manager.ListFiles();   // 获取文件列表服务

    return 0;
}

// 插件测试
void Test_plugin()
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

// 读取配置文件
void Config()
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
        std::cout << "Failed to create config file" << std::endl;
    }
}

// SHA256哈希加密函数（生成64位16进制数）（HEX编码）
std::string Sha256(const std::string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}
