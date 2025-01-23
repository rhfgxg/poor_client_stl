#pragma once
#include <string>
#include <vector>
#include <memory>
#include "plugin_interface.h"

class PluginManager {
public:
    void LoadPlugin(const std::string& path);
    void UnloadPlugins();
    void ExecutePlugins();

private:
    std::vector<std::unique_ptr<IPlugin>> plugins_;
};
