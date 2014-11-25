#include "CNetChannel.hpp"
#include "gm/LuaBindThunk.hpp"
#include "Engine.hpp"

using namespace GarrysMod::Lua;

int CNetChannel::SendNetMsgLua(CLuaInterface& Lua)
{
    // Make sure that the first argument is a net message. All typeids for net messages
    // are between 200 and 250.
    int msgType = Lua.GetType(2);
    if (msgType < 200 || msgType > 250)
        Lua.LuaError("Expected a net message type", 2);

    INetMessage* msg = static_cast<INetMessage*>(Lua.GetUserData(2));
    bool forceReliable = false;

    if (Lua.GetType(3) == Type::BOOL)
        forceReliable = Lua.GetBool(3);

    Lua.Push(SendNetMsg(*msg, forceReliable));

    return 1;
}

int CNetChannel::RequestFileLua(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::STRING);

    int req = RequestFile(Lua.GetString(2));
    Lua.Push(req);

    return 1;
}

// Server only function
int CNetChannel::GetPlayerNetChannelLua(CLuaInterface& Lua)
{
    Lua.CheckType(1, Type::ENTITY);
    CLuaObject playerEnt = Lua.GetObject(1);

    // Get player entity index
    CLuaObject entIndex = Lua.GetMetaTable("Entity", Type::ENTITY).GetMember("EntIndex");
    entIndex.Push();
    playerEnt.Push();

    Lua.Call(1, 1);

    int index = Lua.GetInteger(-1);

    CNetChannel* nc = static_cast<CNetChannel*>(Engine::Server->GetPlayerNetInfo(index));
    Lua.PushBoundObject(nc, false);

    return 1;
}

void CNetChannel::InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
{
    mtIndex.SetMember("SendNetMsg", LuaMemberBindThunk<CNetChannel, &CNetChannel::SendNetMsgLua>);
    mtIndex.SetMember("RequestFile", LuaMemberBindThunk<CNetChannel, &CNetChannel::RequestFileLua>);

    if (Lua.IsServer())
    {
        CLuaObject playerMT = Lua.GetMetaTable("Player", Type::ENTITY);
        playerMT.SetMember("GetNetChannel", LuaStaticBindThunk<&CNetChannel::GetPlayerNetChannelLua>);
    }
}

const char* CNetChannel::LuaMetaTableName = "nethook.CNetChannel";
const int CNetChannel::LuaTypeID = 199;
