#include "plugin_manager.h"
#include <dlfcn.h>  // 提供了动态链接库（DLL）操作的接口。用于在运行时加载和卸载共享库，以及查找符号（函数或变量）的地址
#include <iostream>

// 加载插件文件
void PluginManager::LoadPlugin(const std::string& path)
{
    // dlopen()函数打开指定的动态链接库文件，并返回一个句柄，该句柄可以用于后续的动态链接库操作
    void* handle = dlopen(path.c_str(), RTLD_LAZY); // RTLD_LAZY：在dlopen返回前，不会解析库中的未定义符号
    if(!handle)
    {
        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        return;
    }

    // dlsym()函数在动态链接库中查找符号name，并返回符号的地址
    // 参数：句柄，符号名，返回值：符号（函数或变量）的指针
    auto create_plugin = reinterpret_cast<IPlugin* (*)()>(dlsym(handle, "CreatePlugin"));    
    auto destroy_plugin = reinterpret_cast<void (*)(IPlugin*)>(dlsym(handle, "DestroyPlugin"));

    if(!create_plugin || !destroy_plugin) // 如果未找到符号，则返回NULL
    {
        std::cerr << "Failed to load plugin symbols: " << dlerror() << std::endl;
        dlclose(handle);
        return;
    }

    // 创建插件实例并存储在 unique_ptr 中
    std::unique_ptr<IPlugin> plugin(create_plugin());
    plugins_.push_back(std::move(plugin));
}

// 关闭所有插件
void PluginManager::UnloadPlugins()
{
    plugins_.clear();
}

// 执行所有插件
void PluginManager::ExecutePlugins()
{
    // 遍历插件列表，执行插件
    for(const auto& plugin : plugins_)
    {
        plugin->Initialize();  // 初始化插件
        plugin->Execute();     // 执行插件功能
        plugin->Shutdown();    // 关闭插件，清理资源
    }
}

// 获取插件接口
IPlugin* PluginManager::GetPlugin(size_t index)
{
    if(index < plugins_.size())
    {
        //return plugins_[index].first;
    }
    return nullptr;
}
