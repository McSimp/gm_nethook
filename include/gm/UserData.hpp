#ifndef GARRYSMOD_LUA_USERDATA_HPP
#define GARRYSMOD_LUA_USERDATA_HPP

#include "dbg.h"
#include "CLuaInterface.hpp"

namespace GarrysMod
{
    namespace Lua
    {
        struct UserData
        {
            void*			data;
            unsigned char	type;
            bool            needsGC;
        };
    }
}

#endif 
