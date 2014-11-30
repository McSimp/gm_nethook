#ifndef CHOOKEDNETMESSAGE_HPP
#define CHOOKEDNETMESSAGE_HPP

#include "gm/CStateManager.hpp"
#include "gm/CLuaInterface.hpp"

#define MSG_REGISTRATION(name) MsgRegistration##name

#define IMPLEMENT_MESSAGE_BASECLASS(name, baseclass) \
    class name; \
    CMessageClassRegistration<name> MSG_REGISTRATION(name)(#name); \
    typedef baseclass<name, MSG_REGISTRATION(name)> BaseClass; \
    class name : public baseclass<name, MSG_REGISTRATION(name)>

#define IMPLEMENT_MESSAGE(name) IMPLEMENT_MESSAGE_BASECLASS(name, CHookedNetMessage)

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
                DevMsg("[nethook] Skipping outgoing message %s\n", R.GetMsgName().c_str());
                return true;
            }
        }

        return R.CallOriginalWrite(this, buffer);
    }

    bool ProcessHook()
    {
        for (CLuaInterface& Lua : GarrysMod::Lua::g_StateManager.GetStates())
        {
            const CLuaObject& processCB = Lua.GetNethookProcessCallback();
            if (processCB.IsNil())
                continue;

            processCB.Push();
            Lua.Push(R.GetMsgName().c_str());
            Lua.PushBoundObject(static_cast<T*>(this), false);

            bool ignoreMessage = Lua.CallGetBool(2);
            if (ignoreMessage)
            {
                DevMsg("[nethook] Skipping incoming message %s\n", R.GetMsgName().c_str());
                return true;
            }
        }

        return R.CallOriginalProcess(this);
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
