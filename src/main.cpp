#include "gm/Lua.hpp"
#include "gm/CLuaInterface.hpp"
#include "gm/CStateManager.hpp"
#include "gm/CLuaInterface.hpp"
#include "NetMessageManager.hpp"
#include "gm/LuaBindThunk.hpp"

using namespace GarrysMod::Lua;

int SetCallback(CLuaInterface& Lua)
{
    Lua.CheckType(1, Type::FUNCTION);
    Lua.SetNetHookCallback(Lua.GetObject(1));
    return 0;
}

LUA_FUNCTION(resolve)
{
    UsesLua();

    try
    {
        NetMessageManager::ResolveMessages();
        NetMessageManager::AddMessagesToLua(Lua);
    }
    catch (std::runtime_error e)
    {
        Lua.Error("[nethook] Initialization error: %s", e.what());
    }

    CLuaObject nethookTable = Lua.GetNewTable();
    nethookTable.SetMember("SetCallback", LuaStaticBindThunk<SetCallback>);
    Lua.SetGlobal("nethook", nethookTable);

    return 0;
}

DLL_EXPORT int gmod13_open(lua_State* L)
{
    CLuaInterface& Lua = g_StateManager.SetupState(L);

    resolve(L);
    Msg("[nethook] Loaded\n");

    return 0;
}

DLL_EXPORT int gmod13_close(lua_State* L)
{
    NetMessageManager::UnhookAllMessages();
    g_StateManager.RemoveState(L);
    Msg("[nethook] Unloaded\n");

    return 0;
}
