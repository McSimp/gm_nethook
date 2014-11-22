#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CHookedNetMessage.hpp"
#include "strtools.h"

using namespace GarrysMod::Lua;

IMPLEMENT_MESSAGE(net_StringCmd)
{
public:
    net_StringCmd() { m_szCommand = nullptr; };

    const char* m_szCommand;
    char m_szCommandBuffer[1024];

    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    int GetCommandLua(CLuaInterface& Lua)
    {
        if (m_szCommand == nullptr)
        {
            Lua.PushNil();
        }
        else
        {
            Lua.Push(m_szCommand);
        }

        return 1;
    }

    void SetCommand(const char* cmd)
    {
        V_strcpy_safe(m_szCommandBuffer, cmd);
        m_szCommand = m_szCommandBuffer;
    }

    int SetCommandLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);
        SetCommand(Lua.GetString(2));
        return 0;
    }

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        Lua.CheckType(1, Type::STRING);

        net_StringCmd* obj = MSG_REGISTRATION(net_StringCmd).CreateNewMessage();
        obj->SetCommand(Lua.GetString(1));
        Lua.PushBoundObject(obj);

        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetCommand", LuaMemberBindThunk<net_StringCmd, &net_StringCmd::GetCommandLua>);
        mtIndex.SetMember("SetCommand", LuaMemberBindThunk<net_StringCmd, &net_StringCmd::SetCommandLua>);
    }
};

const char* net_StringCmd::LuaMetaTableName = "nethook.net_StringCmd";
const int net_StringCmd::LuaTypeID = 204;
