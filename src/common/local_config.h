#ifndef LOCAL_CONFIG
#define LOCAL_CONFIG

#include <string>
#include <fstream>

// 本地配置文件数据类
class LocalConfig
{
public:
    static LocalConfig& Get_config();   // 获取配置数据对象

    void Init(const std::string& file_path_); // 初始化配置数据对象
    void Update(const std::string& key_, const std::string& value_); // 更新配置项
    void Update_temporary(const std::string& key_, const std::string& value_);    // 临时更新配置项，但不保存到文件

    // 配置项
    std::string client_version; // 客户端版本
    std::string client_token;   // 客户端令牌

    std::string dir_log;    // 日志目录
    std::string dir_download;   // 下载目录
    std::string dir_recently_open;  // 最近打开目录

    std::string accounts;   // 账号密码列表，格式为 "account1:password1;account2:password2;..."
    std::string current_account; // 当前登录账号

private:
    // 禁用构造和复制函数
    LocalConfig() = default; // 构造函数私有
    LocalConfig(const LocalConfig&) = delete;
    LocalConfig& operator=(const LocalConfig&) = delete;

    void Default_value(); // 配置数据默认值

    std::string file_path; // 配置文件路径
};

#endif // !LOCAL_CONFIG
