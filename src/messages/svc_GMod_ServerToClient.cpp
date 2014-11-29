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
            DevMsg("[nethook] Using m_read for data\n");
            reader = new lua_bf_read(m_read, m_numBits);
        }
        else if (UseWrite())
        {
            DevMsg("[nethook] Using m_write for data\n");
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
        
        if (!UseRead() && !UseWrite())
        {
            Lua.Error("neither m_read nor m_write has m_nDataBits == -1");
        }

        // Copy the data out of the Lua buffer so it can be GC'd.
        m_write.StartWriting(messageData, sizeof(messageData));
        m_write.WriteBits(writer->GetBasePointer(), writer->GetNumBitsWritten());

        if (UseRead())
        {
            // Update members of m_read and reset m_write since it's not meant to be used
            m_read.StartReading(messageData, sizeof(messageData), 0, writer->GetNumBitsWritten());
            m_write = bf_write();
        }

        m_numBits = writer->GetNumBitsWritten();

        return 0;
    }

    int GetNumBitsLua(CLuaInterface& Lua)
    {
        Lua.Push(m_numBits);
        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetData", LuaMemberBindThunk<svc_GMod_ServerToClient, &GetDataLua>);
        mtIndex.SetMember("SetData", LuaMemberBindThunk<svc_GMod_ServerToClient, &SetDataLua>);
        mtIndex.SetMember("GetNumBits", LuaMemberBindThunk<svc_GMod_ServerToClient, &GetNumBitsLua>);
    }
};

const char* svc_GMod_ServerToClient::LuaMetaTableName = "nethook.svc_GMod_ServerToClient";
const int svc_GMod_ServerToClient::LuaTypeID = 201;
