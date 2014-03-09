#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CHookedNetMessage.hpp"
#include "lua_bf_read.hpp"

using namespace GarrysMod::Lua;

class svc_GMod_ServerToClient;
CMessageClassRegistration<svc_GMod_ServerToClient> Registration("svc_GMod_ServerToClient");

class svc_GMod_ServerToClient : public CHookedNetMessage<svc_GMod_ServerToClient, Registration>
{
public:
    svc_GMod_ServerToClient() { m_bReliable = true; };

    int m_numBits;
    bf_read m_read;
    bf_write m_write;

    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        svc_GMod_ServerToClient* obj = Registration.CreateNewMessage();
        Lua.PushBoundObject(obj);

        return 1;
    }

    int GetDataLua(CLuaInterface& Lua)
    {
        lua_bf_read* reader = new lua_bf_read(m_write, m_numBits);
        Lua.PushBoundObject(reader);

        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetData", LuaMemberBindThunk<svc_GMod_ServerToClient, &GetDataLua>);
    }
};

const char* svc_GMod_ServerToClient::LuaMetaTableName = "nethook.svc_GMod_ServerToClient";
const int svc_GMod_ServerToClient::LuaTypeID = 201;
