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
        
        // Workaround for C++'s inability to handle member function pointers to the base class
        // correctly in template arguments. Could use a static_cast to bypass the retardedness,
        // but then this would make Intellisense complain.
        template <class Derived, class Base, int (Base::*F)(CLuaInterface& Lua)>
        int LuaMemberBindThunk(lua_State* L)
        {
            CLuaInterface& Lua = g_StateManager.GetInterface(L);
            Derived* obj = static_cast<Derived*>(Lua.CheckAndGetUserData(1, Derived::LuaTypeID));
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
