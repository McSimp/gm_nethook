#include "gm/Lua.hpp"
#include "gm/CLuaInterface.hpp"
#include "gm/CStateManager.hpp"
#include "gm/CLuaInterface.hpp"
#include "NetMessageManager.hpp"
#include "gm/LuaBindThunk.hpp"

using namespace GarrysMod::Lua;

int SetWriteCallback(CLuaInterface& Lua)
{
    Lua.CheckType(1, Type::FUNCTION);
    Lua.SetNethookWriteCallback(Lua.GetObject(1));
    return 0;
}

int AttachMessage(CLuaInterface& Lua)
{
    Lua.CheckType(1, Type::STRING);
    
    if (!NetMessageManager::AttachMessage(Lua.GetString(1)))
    {
        Lua.Error("Failed to attach to net message");
    }

    return 0;
}

int DetachMessage(CLuaInterface& Lua)
{
    Lua.CheckType(1, Type::STRING);

    if (!NetMessageManager::DetachMessage(Lua.GetString(1)))
    {
        Lua.Error("Failed to detach from net message");
    }

    return 0;
}

void InitializeNethook(CLuaInterface& Lua)
{
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
    nethookTable.SetMember("SetWriteCallback", LuaStaticBindThunk<SetWriteCallback>);
    nethookTable.SetMember("AttachMessage", LuaStaticBindThunk<AttachMessage>);
    nethookTable.SetMember("DetachMessage", LuaStaticBindThunk<DetachMessage>);
    Lua.SetGlobal("nethook", nethookTable);
}

DLL_EXPORT int gmod13_open(lua_State* L)
{
    CLuaInterface& Lua = g_StateManager.SetupState(L);
    InitializeNethook(Lua);
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
