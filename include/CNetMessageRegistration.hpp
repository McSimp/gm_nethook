#ifndef CNETMESSAGEREGISTRATION_HPP
#define CNETMESSAGEREGISTRATION_HPP

#include <string>
#include <map>
#include "CSimpleDetour.hpp"
#include "gm/CLuaInterface.hpp"
#include "inetmessage.h"
#include "FuncPtr.hpp"

typedef void (*tLuaSetupFunc) (GarrysMod::Lua::CLuaInterface& Lua);
typedef bool (__thiscall* tMsgWriteFunc) (INetMessage* msg, bf_write& buffer);

class CNetMessageRegistration
{
    void** m_vtable;

    FuncPtr<tMsgWriteFunc> m_writeOriginalFunc;
    FuncPtr<tMsgWriteFunc> m_writeHookFunc;
    CSimpleDetour<tMsgWriteFunc> m_writeDetour;

    FuncPtr<tLuaSetupFunc> m_luaSetupFunc;

public:
    static std::map<std::string, CNetMessageRegistration&> RegisteredMessages;

    CNetMessageRegistration(const std::string& msgName, FuncPtr<tMsgWriteFunc> writeHookFunc, FuncPtr<tLuaSetupFunc> luaSetupFunc);
    bool IsResolved();
    void ResolveFromVTable(void** vtable);
    void InitializeLua(GarrysMod::Lua::CLuaInterface& Lua);

    bool CallOriginalWrite(INetMessage* msg, bf_write& buffer);

    void Attach();
    void Detach();
};

template <class T>
class CMessageClassRegistration : public CNetMessageRegistration
{
public:
    CMessageClassRegistration(const std::string& msgName)
        : CNetMessageRegistration(msgName, MakeFuncPtr<tMsgWriteFunc>(&T::WriteHook), MakeFuncPtr<tLuaSetupFunc>(&T::InitializeLua))
    {}
};

#endif
