#include "CNetMessageRegistration.hpp"
#include "NetMessageManager.hpp"

using namespace GarrysMod::Lua;

std::map<std::string, CNetMessageRegistration&> CNetMessageRegistration::RegisteredMessages;

CNetMessageRegistration::CNetMessageRegistration(const std::string& msgName, FuncPtr<tMsgWriteFunc> writeHookFunc, FuncPtr<tLuaSetupFunc> luaSetupFunc)
    : m_writeHookFunc(writeHookFunc)
	, m_writeDetour(&m_writeOriginalFunc, m_writeHookFunc)
	, m_luaSetupFunc(luaSetupFunc)
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
	
	//m_processOriginalFunc = static_cast<tMsgProcessFunc>(m_vtable[3]);
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

void CNetMessageRegistration::Attach()
{
	m_writeDetour.Attach();
}

void CNetMessageRegistration::Detach()
{
	m_writeDetour.Detach();
}
