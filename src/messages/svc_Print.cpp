#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"

using namespace GarrysMod::Lua;

class SVC_Print;
CMessageClassRegistration<SVC_Print> Registration("svc_Print");

class SVC_Print : public CNetMessage
{
    SVC_Print() { m_bReliable = false; m_text = nullptr; };

public:
    void* m_pMessageHandler;
    const char* m_text;

    bool WriteHook(bf_write& buffer)
    {
        return Registration.CallOriginalWrite(this, buffer);
    }

    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    int GetTextLua(CLuaInterface& Lua)
    {
        if (m_text == nullptr)
            Lua.PushNil();
        else
            Lua.Push(this->m_text);

        return 1;
    }

    int SetTextLua(CLuaInterface& Lua)
    {
        Lua.CheckType(1, Type::STRING);
        m_text = Lua.GetString(1);
        return 0;
    }

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        Lua.CheckType(1, Type::STRING);
        
        SVC_Print* obj = new SVC_Print();
        obj->m_text = Lua.GetString(1);
        Lua.PushBoundObject(obj);

        return 1;
    }

    static void InitializeLua(CLuaInterface& Lua)
    {
        Msg("svc_Print is being added to Lua!\n");
        Registration.Attach();

        // Setup metatable for this class
        CLuaObject mt = Lua.GetMetaTable(LuaMetaTableName, LuaTypeID);
        CLuaObject __index = Lua.GetNewTable();
        __index.SetMember("GetText", &LuaMemberBindThunk<SVC_Print, &SVC_Print::GetTextLua>);
        __index.SetMember("SetText", &LuaMemberBindThunk<SVC_Print, &SVC_Print::SetTextLua>);
        mt.SetMember("__index", __index);

        Lua.SetGlobal("SVC_Print", &LuaStaticBindThunk<&SVC_Print::LuaCreateObject>);
    }
};

const char* SVC_Print::LuaMetaTableName = "NETHOOK::SVC_Print";
const int SVC_Print::LuaTypeID = 205;
