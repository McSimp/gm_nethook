#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CHookedNetMessage.hpp"

using namespace GarrysMod::Lua;

class svc_Print;
CMessageClassRegistration<svc_Print> Registration("svc_Print");

class svc_Print : public CHookedNetMessage<svc_Print, Registration>
{
    svc_Print() { m_bReliable = false; m_text = nullptr; };

public:
    void* m_pMessageHandler;
    const char* m_text;
    
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
        // TODO: Need to copy this string
        m_text = Lua.GetString(2);
        return 0;
    }

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        Lua.CheckType(1, Type::STRING);
        
        svc_Print* obj = new svc_Print();
        // TODO: Need to copy this string
        obj->m_text = Lua.GetString(1);
        Lua.PushBoundObject(obj);

        return 1;
    }

    static void InitializeLua(CLuaInterface& Lua)
    {
        CLuaObject mt = Lua.GetMetaTable(LuaMetaTableName, LuaTypeID);
        CLuaObject __index = Lua.GetNewTable();
        __index.SetMember("GetText", LuaMemberBindThunk<svc_Print, &svc_Print::GetTextLua>);
        __index.SetMember("SetText", LuaMemberBindThunk<svc_Print, &svc_Print::SetTextLua>);
        mt.SetMember("__index", __index);

        //Lua.SetGlobal("svc_Print", LuaStaticBindThunk<&svc_Print::LuaCreateObject>);
    }
};

const char* svc_Print::LuaMetaTableName = "nethook.svc_Print";
const int svc_Print::LuaTypeID = 205;
