#include "local_config.h"
#include <fstream>
#include <sstream>

// 获取配置数据对象
LocalConfig& LocalConfig::Get_config()
{
    static LocalConfig instance; // 懒加载，线程安全
    return instance;
}

// 初始化配置数据对象
void LocalConfig::Init(const std::string& file_path_)
{
/*
* 打开并读取配置文件内容，保存字段值到对应成员中
* 如果配置文件不存在，则创建一个新的配置文件，并使用默认值初始化各个字段
* 如果配置文件缺少某些字段，则使用默认值初始化这些字段，不保存到配置文件
* 如果配置文件中某个字段的值为空，则将其初始化为默认值，不保存到配置文件
*/

    file_path = file_path_; // 保存配置文件路径

    Default_value(); // 为数据设置默认值

    std::ifstream in(file_path);
    std::string line;
    while(std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string key, value;
        if(std::getline(iss, key, '=') && std::getline(iss, value))
        {
            if(key == "client_version") client_version = value;
            // else if(key == "client_token") client_token = std::stoi(value);  // 转整型
            else if(key == "client_token") client_token = value;

            else if(key == "dir_log") dir_log = value;
            else if(key == "dir_download") dir_download = value;
            else if(key == "dir_recently_open") dir_recently_open = value;

            else if(key == "accounts") accounts = value;
            else if(key == "current_account") current_account = value;
        }
    }
}

// 为配置数据设置默认值
void LocalConfig::Default_value()
{
    client_version = "0";
    client_token = "";

    dir_log = "./logs";
    dir_download = "./downloads";
    dir_recently_open = "";

    accounts = "";
    current_account = "";
}

// 更新配置数据对象
void LocalConfig::Update(const std::string& key, const std::string& value)
{
    if(key == "client_version") client_version = value;
    else if(key == "client_token") client_token = value;

    else if(key == "dir_log") dir_log = value;
    else if(key == "dir_download") dir_download = value;
    else if(key == "dir_recently_open") dir_recently_open = value;

    else if(key == "accounts") accounts = value;
    else if(key == "current_account") current_account = value;

    // 保存到配置文件
    std::ofstream out(file_path, std::ios::app);
    if(out.is_open())
    {
        out << key << "=" << value << std::endl;
        out.close();
    }
    else
    {
        // 处理文件打开失败的情况
        throw std::runtime_error("Failed to open config file for writing.");
    }
}

// 临时更新配置项，但不保存到文件
void LocalConfig::Update_temporary(const std::string& key, const std::string& value)
{
    // 临时更新配置项
    if(key == "client_version") client_version = value;
    else if(key == "client_token") client_token = value;

    else if(key == "dir_log") dir_log = value;
    else if(key == "dir_download") dir_download = value;
    else if(key == "dir_recently_open") dir_recently_open = value;

    else if(key == "accounts") accounts = value;
    else if(key == "current_account") current_account = value;
}
