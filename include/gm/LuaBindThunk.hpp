#ifndef LUABINDTHUNK_HPP
#define LUABINDTHUNK_HPP

#include "CLuaInterface.hpp"
#include "CStateManager.hpp"

namespace GarrysMod
{
    namespace Lua
    {
        template <class T, int (T::*F)(CLuaInterface& Lua)>
        int LuaMemberBindThunk(lua_State* L)
        {
            CLuaInterface& Lua = g_StateManager.GetInterface(L);
            T* obj = static_cast<T*>(Lua.CheckAndGetUserData(1, T::LuaTypeID));
            return (obj->*F)(Lua);
        }

        template <int (*F)(CLuaInterface& Lua)>
        int LuaStaticBindThunk(lua_State* L)
        {
            CLuaInterface& Lua = g_StateManager.GetInterface(L);
            return F(Lua);
        }
    }
}

#endif
