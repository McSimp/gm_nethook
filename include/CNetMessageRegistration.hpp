#ifndef CNETMESSAGEREGISTRATION_HPP
#define CNETMESSAGEREGISTRATION_HPP

#include <string>
#include <map>
#include "CSimpleDetour.hpp"
#include "gm/CLuaInterface.hpp"
#include "inetmessage.h"
#include "FuncPtr.hpp"
#include "CVirtualHook.hpp"

typedef void (*tLuaSetupFunc) (GarrysMod::Lua::CLuaInterface& Lua);
typedef bool (__thiscall* tMsgWriteFunc) (INetMessage* msg, bf_write& buffer);
typedef bool (__thiscall* tMsgProcessFunc) (INetMessage* msg);

class CNetMessageRegistration
{
    FuncPtr<tMsgWriteFunc> m_writeOriginalFunc;
    FuncPtr<tMsgWriteFunc> m_writeHookFunc;
    CSimpleDetour<tMsgWriteFunc> m_writeDetour;

    FuncPtr<tMsgProcessFunc> m_processOriginalFunc;
    FuncPtr<tMsgProcessFunc> m_processHookFunc;
    CSimpleDetour<tMsgProcessFunc> m_processDetour;

    FuncPtr<tLuaSetupFunc> m_luaSetupFunc;

    std::string m_msgName;

protected:
    void** m_vtable;

public:
    static std::map<std::string, CNetMessageRegistration&> RegisteredMessages;

    CNetMessageRegistration(const std::string& msgName, FuncPtr<tMsgWriteFunc> writeHookFunc, FuncPtr<tMsgProcessFunc> processHookFunc, FuncPtr<tLuaSetupFunc> luaSetupFunc);
    bool IsResolved();
    void ResolveFromVTable(void** vtable);
    void InitializeLua(GarrysMod::Lua::CLuaInterface& Lua);

    bool CallOriginalWrite(INetMessage* msg, bf_write& buffer);
    bool CallOriginalProcess(INetMessage* msg);

    void Attach();
    void Detach();

    const std::string& GetMsgName();
};

template <class T>
class CMessageClassRegistration : public CNetMessageRegistration
{
public:
    CMessageClassRegistration(const std::string& msgName)
        : CNetMessageRegistration(msgName, MakeFuncPtr<tMsgWriteFunc>(&T::WriteHook), MakeFuncPtr<tMsgProcessFunc>(&T::ProcessHook), MakeFuncPtr<tLuaSetupFunc>(&T::InitializeLua))
    {}

    T* CreateNewMessage()
    {
        if (!IsResolved())
        {
            return nullptr;
        }

        T* obj = new T();

        // Replace the vtable with the engine's vtable
        void*** pVtable = reinterpret_cast<void***>(obj);
        pVtable[0] = m_vtable;

        return obj;
    }
};

#endif
