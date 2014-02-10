#include "gm/Lua.hpp"
#include "gm/CLuaInterface.hpp"
#include "gm/CStateManager.hpp"
#include "NetMessageManager.hpp"

using namespace GarrysMod::Lua;

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
