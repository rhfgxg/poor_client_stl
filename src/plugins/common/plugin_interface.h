#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual void Initialize() = 0;
    virtual void Execute() = 0;
    virtual void Shutdown() = 0;
};

extern "C" {
    IPlugin* CreatePlugin();
    void DestroyPlugin(IPlugin* plugin);
}

#endif // PLUGIN_INTERFACE_H
