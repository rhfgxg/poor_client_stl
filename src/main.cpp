#include "plugin_manager.h"     // 插件管理器
#include "file_plugin.h"   // 文件模块插件
#include "user_manager.h"   // 用户管理器
#include "gateway_manager.h"    // 网关管理器
#include "file_manager.h"   // 文件管理器
#include "local_config.h"  // 本地配置文件数据类

#include <string>
#include <fstream>
#include <iostream>

#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

void Init_client();  //初始化客户端
void Self_inspection(); // 自检函数
std::string Sha256(const std::string& str); // SHA256哈希加密函数（生成64位16进制数）（HEX编码）
void Test();   // 测试函数
void Test_plugin();   // 插件测试

// 主模块 main函数
int main()
{
    Init_client();   // 读取客户端配置文件，初始化客户端配置

    Test();  // 测试函数
    return 0;
}

// 初始化客户端
void Init_client()
{
    Self_inspection(); // 自检函数，环境及资源自检

    LocalConfig::Get_config().Init("./config/config.ini"); // 初始化配置数据对象
}

// 自检函数
void Self_inspection()
{
/*
* 自检函数：检查必要的文件夹和配置文件是否存在
*/

// 必须文件夹检查
    // 需要检查的文件夹列表
    std::vector<std::string> folders = {
        "./config", // 配置文件夹
        "./log",    // 默认日志文件夹
        "./download",    // 默认下载文件夹
        "./cache/upload/file",      // 上传文件缓存
        "./cache/download/file",    // 下载文件缓存
        "./cache/local"            // 本地缓存
    };

    for(const auto& folder : folders)
    {
        std::error_code ec;
        if(!std::filesystem::exists(folder))    // 文件夹不存在时创建
        {
            if(std::filesystem::create_directories(folder, ec)) // 创建文件夹
            {
                std::cout << "已创建文件夹: " << folder << std::endl;
            }
            else if(ec)
            {
                std::cerr << "创建文件夹失败: " << folder << "，错误: " << ec.message() << std::endl;
            }
        }
    }

// 文件检查
    // 配置文件
    // 配置文件异常时，自动生成默认配置文件（使用联网下载可能会出现下载错误）
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

// 测试函数
void Test()
{
    // 基础数据和对象初始化
    const std::string user_name = "lhw";
    const std::string account = "3056078308";
    const std::string password = "159357";
    std::string hashed_password = Sha256(password); // 生成64位 16进制 哈希密码
    const std::string email = "3056078308@qq.com";
    const std::string phone = "";
    const std::string id_number = "";

    GatewayManager gateway_manager; // 网关管理器，单例
    UserManager user_manager(gateway_manager);  // 用户管理器
    FileManager file_manager(gateway_manager, user_manager);    // 文件管理器

    // 用户相关测试
    //user_manager.Register(user_name, hashed_password, email, phone, id_number);    // 注册
    //user_manager.Login(account, hashed_password);   // 登录
    //user_manager.Logout();   // 登出
    //user_manager.Change_password(hashed_password, sha256("123456"));    // 修改密码

    // 网盘相关测试
    // file_manager.Upload("./download/_DSC0984.ARW");  // 文件上传服务
    // file_manager.Download("_DSC0984.ARW");  // 文件下载服务
    // file_manager.Delete("_DSC0984.ARW");    // 文件删除服务 未实现
    // file_manager.ListFiles();   // 获取文件列表服务 未实现
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

