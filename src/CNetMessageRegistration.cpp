#include "CNetMessageRegistration.hpp"
#include "NetMessageManager.hpp"

using namespace GarrysMod::Lua;

std::map<std::string, CNetMessageRegistration&> CNetMessageRegistration::RegisteredMessages;

CNetMessageRegistration::CNetMessageRegistration(const std::string& msgName, FuncPtr<tMsgWriteFunc> writeHookFunc, FuncPtr<tMsgProcessFunc> processHookFunc, FuncPtr<tLuaSetupFunc> luaSetupFunc)
    : m_writeHookFunc(writeHookFunc)
    , m_writeDetour(&m_writeOriginalFunc, m_writeHookFunc)
    , m_processHookFunc(processHookFunc)
    , m_processDetour(&m_processOriginalFunc, m_processHookFunc)
    , m_luaSetupFunc(luaSetupFunc)
    , m_msgName(msgName)
{
    RegisteredMessages.emplace(msgName, *this);
}

bool CNetMessageRegistration::IsResolved()
{
    return m_vtable != nullptr;
}

void CNetMessageRegistration::ResolveFromVTable(void** vtable)
{
    m_vtable = vtable;

    m_processOriginalFunc.voidPtr = m_vtable[3];
    m_writeOriginalFunc.voidPtr = m_vtable[5];

    // TODO: Check these are actual functions (should start with push ebp or a mov)
}

void CNetMessageRegistration::InitializeLua(CLuaInterface& Lua)
{
    m_luaSetupFunc.func(Lua);
}

bool CNetMessageRegistration::CallOriginalWrite(INetMessage* msg, bf_write& buffer)
{
    return m_writeOriginalFunc.func(msg, buffer);
}

bool CNetMessageRegistration::CallOriginalProcess(INetMessage* msg)
{
    return m_processOriginalFunc.func(msg);
}

void CNetMessageRegistration::Attach()
{
    m_writeDetour.Attach();
    m_processDetour.Attach();
}

void CNetMessageRegistration::Detach()
{
    m_writeDetour.Detach();
    m_processDetour.Detach();
}

const std::string& CNetMessageRegistration::GetMsgName()
{
    return m_msgName;
}
