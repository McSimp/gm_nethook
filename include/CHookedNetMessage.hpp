#ifndef CHOOKEDNETMESSAGE_HPP
#define CHOOKEDNETMESSAGE_HPP

#include "gm/CStateManager.hpp"
#include "gm/CLuaInterface.hpp"

#define MSG_REGISTRATION(name) MsgRegistration##name

#define IMPLEMENT_MESSAGE(name) \
    class name; \
    CMessageClassRegistration<name> MSG_REGISTRATION(name)(#name); \
    class name : public CHookedNetMessage<name, MSG_REGISTRATION(name)>

template <typename T, CMessageClassRegistration<T>& R>
class CHookedNetMessage : public CNetMessage
{
public:
    bool WriteHook(bf_write& buffer)
    {
        for (CLuaInterface& Lua : GarrysMod::Lua::g_StateManager.GetStates())
        {
            const CLuaObject& writeCB = Lua.GetNethookWriteCallback();
            if (writeCB.IsNil())
                continue;

            writeCB.Push();
            Lua.Push(R.GetMsgName().c_str());
            Lua.PushBoundObject(static_cast<T*>(this), false);

            bool ignoreMessage = Lua.CallGetBool(2);
            if (ignoreMessage)
            {
#ifdef _DEBUG
                Msg("[nethook] Skipping message %s\n", R.GetMsgName().c_str());
#endif
                return true;
            }
        }

        return R.CallOriginalWrite(this, buffer);
    }

    static void InitializeLua(GarrysMod::Lua::CLuaInterface& Lua)
    {
        Lua.InitializeClass<T>();

        Lua.GetGlobal("nethook")
           .GetMember("MessageFactory")
           .SetMember(R.GetMsgName().c_str(), LuaStaticBindThunk<&T::LuaCreateObject>);
    }
};

#endif
