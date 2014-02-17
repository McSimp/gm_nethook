#pragma comment (linker, "/NODEFAULTLIB:libcmt")

#include "gm/Lua.hpp"
#include "gm/CLuaInterface.hpp"
#include "gm/CStateManager.hpp"
#include "gm/CLuaInterface.hpp"
#include "NetMessageManager.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CNetChannel.hpp"
#include "Engine.hpp"

using namespace GarrysMod::Lua;

void InitializeNethook(CLuaInterface& Lua)
{
    if (Lua.IsServer())
        Engine::InitializeServer();
    else
        Engine::InitializeClient();

    try
    {
        NetMessageManager::ResolveMessages();
    }
    catch (std::runtime_error e)
    {
        Lua.Error("[nethook] Initialization error: %s", e.what());
    }

    CLuaObject nethookTable = Lua.GetNewTable();
    nethookTable.SetMember("SetWriteCallback", LuaStaticBindThunk<NetMessageManager::SetWriteCallbackLua>);
    nethookTable.SetMember("AttachMessage", LuaStaticBindThunk<NetMessageManager::AttachMessageLua>);
    nethookTable.SetMember("DetachMessage", LuaStaticBindThunk<NetMessageManager::DetachMessageLua>);
    nethookTable.SetMember("MessageFactory", Lua.GetNewTable());
    Lua.SetGlobal("nethook", nethookTable);

    // TODO: Perhaps do a similar thing as I did with the net messages, create an
    // object which has its constructor called when the library is loaded. 
    CNetChannel::InitializeLua(Lua);

    NetMessageManager::AddMessagesToLua(Lua);
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
