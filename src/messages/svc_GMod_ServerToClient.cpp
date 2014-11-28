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

    bool UseWrite()
    {
        return m_read.m_nDataBits == -1;
    }

    bool UseRead()
    {
        return m_write.m_nDataBits == -1;
    }

    int GetDataLua(CLuaInterface& Lua)
    {
        // Return whichever bitbuf has the actual data in it
        lua_bf_read* reader = nullptr;

        if (UseRead())
        {
#ifdef _DEBUG
            Msg("[nethook] Using m_read for data\n");
#endif
            reader = new lua_bf_read(m_read, m_numBits);
        }
        else if (UseWrite())
        {
#ifdef _DEBUG
            Msg("[nethook] Using m_write for data\n");
#endif
            reader = new lua_bf_read(m_write, m_numBits);
        }
        else
        {
            Lua.Error("neither m_read nor m_write has m_nDataBits == -1");
        }

        Lua.PushBoundObject(reader);
        return 1;
    }

    int SetDataLua(CLuaInterface& Lua)
    {
        static char messageData[102400];

        lua_bf_write* writer = static_cast<lua_bf_write*>(Lua.CheckAndGetUserData(2, lua_bf_write::LuaTypeID));
        
        // Copy the data out of the Lua buffer so it can be GC'd.
        m_write.StartWriting(messageData, sizeof(messageData));
        m_write.WriteBits(writer->GetBasePointer(), writer->GetNumBitsWritten());

        // Update pointers for m_read as well
        m_read.StartReading(messageData, sizeof(messageData), 0, writer->GetNumBitsWritten());

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
