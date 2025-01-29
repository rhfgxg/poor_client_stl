#include "plugin_manager.h"
#include "logger_manager.h"

// 主模块 main函数
int main() {
    LoggerManager logger_manager;
    PluginManager plugin_manager;

    // 加载编译后的插件
    // TODO：不同平台的插件文件名不同，后续进行修改
    plugin_manager.LoadPlugin("./plugins/file_plugin.dll");

    // 执行插件
    plugin_manager.ExecutePlugins();

    // 卸载插件
    plugin_manager.UnloadPlugins();

    return 0;
}
