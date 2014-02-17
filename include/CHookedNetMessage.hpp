#ifndef CHOOKEDNETMESSAGE_HPP
#define CHOOKEDNETMESSAGE_HPP

#include "gm/CStateManager.hpp"
#include "gm/CLuaInterface.hpp"

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
            Lua.PushBoundObject(static_cast<T*>(this));

            // If the hook.Call succeeds
            if (Lua.PCall(2, 1) != 0)
                continue;

            // See if we got a bool as the return value
            if (Lua.GetType(-1) != Type::BOOL)
            {
                Lua.Pop();
                continue;
            }

            bool ignoreMessage = Lua.GetBool();
            Lua.Pop();
            if (ignoreMessage)
                return true;
        }

        return R.CallOriginalWrite(this, buffer);
    }

    static void InitializeLua(GarrysMod::Lua::CLuaInterface& Lua)
    {
        CLuaObject mt = Lua.GetMetaTable(T::LuaMetaTableName, T::LuaTypeID);
        CLuaObject __index = Lua.GetNewTable();
        T::InitializeMetaFunctions(Lua, __index);
        mt.SetMember("__index", __index);

        Lua.GetGlobal("nethook")
           .GetMember("MessageFactory")
           .SetMember(R.GetMsgName().c_str(), LuaStaticBindThunk<&T::LuaCreateObject>);
    }
};

#endif
