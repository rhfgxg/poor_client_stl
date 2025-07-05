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
    file_path = file_path_; // 保存配置文件路径

    std::ifstream in(file_path);
    std::string line;
    // 打开并读取文件内容
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

        }
    }
}

// 更新配置数据对象
void LocalConfig::Update(const std::string& key, const std::string& value)
{
    if(key == "client_version") client_version = value;
    else if(key == "client_token") client_token = value;

    else if(key == "dir_log") dir_log = value;
    else if(key == "dir_download") dir_download = value;
    else if(key == "dir_recently_open") dir_recently_open = value;

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
}
