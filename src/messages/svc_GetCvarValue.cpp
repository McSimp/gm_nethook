#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CHookedNetMessage.hpp"
#include "strtools.h"

using namespace GarrysMod::Lua;

IMPLEMENT_MESSAGE(svc_GetCvarValue)
{
public:
    svc_GetCvarValue() { m_szCvarName = nullptr; };

    int m_iCookie;
    const char *m_szCvarName;
    char m_szCvarNameBuffer[256];

    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    int GetCookieLua(CLuaInterface& Lua)
    {
        Lua.Push(m_iCookie);
        return 1;
    }

    int SetCookieLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_iCookie = Lua.GetInteger(2);
        return 0;
    }

    int GetCvarNameLua(CLuaInterface& Lua)
    {
        if (m_szCvarName == nullptr)
        {
            Lua.PushNil();
        }
        else
        {
            Lua.Push(m_szCvarName);
        }

        return 1;
    }

    void SetCvarName(const char* name)
    {
        V_strcpy_safe(m_szCvarNameBuffer, name);
        m_szCvarName = m_szCvarNameBuffer;
    }

    int SetCvarNameLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);
        SetCvarName(Lua.GetString(2));
        return 0;
    }

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        Lua.CheckType(1, Type::STRING);
        Lua.CheckType(2, Type::NUMBER);

        svc_GetCvarValue* obj = MSG_REGISTRATION(svc_GetCvarValue).CreateNewMessage();
        obj->SetCvarName(Lua.GetString(1));
        obj->m_iCookie = Lua.GetInteger(2);
        Lua.PushBoundObject(obj);

        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetCookie", LuaMemberBindThunk<svc_GetCvarValue, &svc_GetCvarValue::GetCookieLua>);
        mtIndex.SetMember("SetCookie", LuaMemberBindThunk<svc_GetCvarValue, &svc_GetCvarValue::SetCookieLua>);
        mtIndex.SetMember("GetCvarName", LuaMemberBindThunk<svc_GetCvarValue, &svc_GetCvarValue::GetCvarNameLua>);
        mtIndex.SetMember("SetCvarName", LuaMemberBindThunk<svc_GetCvarValue, &svc_GetCvarValue::SetCvarNameLua>);
    }
};

const char* svc_GetCvarValue::LuaMetaTableName = "nethook.svc_GetCvarValue";
const int svc_GetCvarValue::LuaTypeID = 203;
