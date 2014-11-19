#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CHookedNetMessage.hpp"
#include "lua_bf_read.hpp"
#include "lua_bf_write.hpp"

using namespace GarrysMod::Lua;

IMPLEMENT_MESSAGE(svc_GMod_ServerToClient)
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
        svc_GMod_ServerToClient* obj = MSG_REGISTRATION(svc_GMod_ServerToClient).CreateNewMessage();
        Lua.PushBoundObject(obj);
        return 1;
    }

    int GetDataLua(CLuaInterface& Lua)
    {
        lua_bf_read* reader = new lua_bf_read(m_write, m_numBits);
        Lua.PushBoundObject(reader);
        return 1;
    }

    int SetDataLua(CLuaInterface& Lua)
    {
        static char messageData[102400];

        lua_bf_write* writer = static_cast<lua_bf_write*>(Lua.CheckAndGetUserData(2, lua_bf_write::LuaTypeID));
        
        // Copy the data out of the Lua buffer so it can be GC'd.
        m_write.StartWriting(messageData, 102400);
        m_write.WriteBits(writer->GetBasePointer(), writer->GetNumBitsWritten());
        m_numBits = writer->GetNumBitsWritten();

        return 0;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetData", LuaMemberBindThunk<svc_GMod_ServerToClient, &GetDataLua>);
        mtIndex.SetMember("SetData", LuaMemberBindThunk<svc_GMod_ServerToClient, &SetDataLua>);
    }
};

const char* svc_GMod_ServerToClient::LuaMetaTableName = "nethook.svc_GMod_ServerToClient";
const int svc_GMod_ServerToClient::LuaTypeID = 201;
