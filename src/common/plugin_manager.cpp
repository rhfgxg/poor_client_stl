#include "plugin_manager.h"
#include <dlfcn.h>
#include <iostream>

void PluginManager::LoadPlugin(const std::string& path) {
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if(!handle) {
        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        return;
    }

    auto createPlugin = reinterpret_cast<IPlugin* (*)()>(dlsym(handle, "CreatePlugin"));
    auto destroyPlugin = reinterpret_cast<void (*)(IPlugin*)>(dlsym(handle, "DestroyPlugin"));

    if(!createPlugin || !destroyPlugin) {
        std::cerr << "Failed to load plugin symbols: " << dlerror() << std::endl;
        dlclose(handle);
        return;
    }

    IPlugin* plugin = createPlugin();
    plugins_.emplace_back(plugin, destroyPlugin);
}

void PluginManager::UnloadPlugins() {
    plugins_.clear();
}

void PluginManager::ExecutePlugins() {
    for(const auto& plugin : plugins_) {
        plugin->Initialize();
        plugin->Execute();
        plugin->Shutdown();
    }
}
