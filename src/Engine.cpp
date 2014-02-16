#include "Engine.hpp"

namespace Engine
{
    IVEngineServer* Server = nullptr;
    IVEngineClient* Client = nullptr;

    void InitializeServer()
    {
        if (!Server)
        {
            CreateInterfaceFn engineFactory = Sys_GetFactory("engine.dll");
            Server = static_cast<IVEngineServer*>(engineFactory(INTERFACEVERSION_VENGINESERVER, NULL));
        }
    }

    void InitializeClient()
    {
        if (!Client)
        {
            CreateInterfaceFn engineFactory = Sys_GetFactory("engine.dll");
            Client = static_cast<IVEngineClient*>(engineFactory(VENGINE_CLIENT_INTERFACE_VERSION, NULL));
        }
    }
}
