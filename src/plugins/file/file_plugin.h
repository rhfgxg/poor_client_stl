#ifndef FILE_PLUGIN_H
#define FILE_PLUGIN_H

#include "plugin_interface.h"
#include "file_server.h"

class FilePlugin: public IPlugin {
public:
    FilePlugin();
    ~FilePlugin();

    void Initialize() override;
    void Execute() override;
    void Shutdown() override;

private:
    FileServerImpl* file_server;
};

extern "C" IPlugin* CreatePlugin();
extern "C" void DestroyPlugin(IPlugin* plugin);

#endif // FILE_PLUGIN_H
