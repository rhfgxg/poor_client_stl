#pragma once
#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "plugin_interface.h"

class PluginManager
{
public:
    void LoadPlugin(const std::string& path);   // 传入插件路径，加载插件文件
    void UnloadPlugins();   // 卸载所有插件
    void ExecutePlugins();  // 执行所有插件
    IPlugin* GetPlugin(size_t index);    // 获取插件接口

private:
    std::vector<std::unique_ptr<IPlugin>> plugins_; // 插件对象列表
};

#endif // !PLUGIN_MANAGER_H
