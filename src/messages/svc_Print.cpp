#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CHookedNetMessage.hpp"
#include "strtools.h"

using namespace GarrysMod::Lua;

IMPLEMENT_MESSAGE(svc_Print)
{
public:
    svc_Print() { m_bReliable = false; m_text = nullptr; };

    const char* m_text;
    char m_textBuffer[2048];

    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    int GetTextLua(CLuaInterface& Lua)
    {
        if (m_text == nullptr)
            Lua.PushNil();
        else
            Lua.Push(m_text);

        return 1;
    }

    int SetTextLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);
        V_strcpy_safe(m_textBuffer, Lua.GetString(2));
        m_text = m_textBuffer;
        return 0;
    }

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        Lua.CheckType(1, Type::STRING);
        
        svc_Print* obj = MSG_REGISTRATION(svc_Print).CreateNewMessage();
        V_strcpy_safe(obj->m_textBuffer, Lua.GetString(1));
        obj->m_text = obj->m_textBuffer;
        Lua.PushBoundObject(obj);

        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetText", LuaMemberBindThunk<svc_Print, &svc_Print::GetTextLua>);
        mtIndex.SetMember("SetText", LuaMemberBindThunk<svc_Print, &svc_Print::SetTextLua>);
    }
};

const char* svc_Print::LuaMetaTableName = "nethook.svc_Print";
const int svc_Print::LuaTypeID = 200;
