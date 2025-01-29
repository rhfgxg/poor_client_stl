#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

// 插件接口类：插件基类，要求所有插件实现 初始化，执行，关闭 三个方法
class IPlugin
{
public:
    virtual ~IPlugin() = default;   // 虚析构函数
    virtual void Initialize() = 0;  // 纯虚：初始化函数
    virtual void Execute() = 0;     // 纯虚：执行函数
    virtual void Shutdown() = 0;    // 纯虚：关闭函数
};

extern "C"  // 按照C语言的方式来处理这两个函数的名称。这是为了确保这些函数在C++代码中可以被C语言代码调用，或者从动态链接库（DLL）中导出时可以被其他语言调用。C++编译器会对函数名进行“名称修饰”（name mangling），而C语言不会，通过extern "C"可以避免这种修饰，使得函数名在链接时能够正确匹配。
{
    #ifdef WIN32
    __declspec(dllexport) IPlugin* CreatePlugin();    // 创建并返回 插件接口 对象
    __declspec(dllexport) void DestroyPlugin(IPlugin* plugin);    // 销毁 插件接口 对象
    #endif // WIN32
}

#endif // PLUGIN_INTERFACE_H
