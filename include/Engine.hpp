#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <eiface.h>
#include <cdll_int.h>

namespace Engine
{
    extern IVEngineServer* Server;
    extern IVEngineClient* Client;

    void InitializeServer();
    void InitializeClient();
}

#endif
