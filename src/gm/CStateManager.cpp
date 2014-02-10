#include "CStateManager.hpp"

using namespace GarrysMod::Lua;

CStateManager::CStateManager()
{
    m_states.reserve(2); // Probably just going to be client and server
}

CLuaInterface& CStateManager::SetupState(lua_State* L)
{
    // If we've already added it, just return the existing interface
    for (auto& entry : m_states)
    {
        if (entry.GetLuaState() == L)
            return entry;
    }

    // This creates a CLuaInterface
    m_states.emplace_back(L);
    return m_states.back();
}

void CStateManager::RemoveState(lua_State* L)
{
    std::vector<CLuaInterface>::size_type i = 0;
    while (i < m_states.size())
    {
        if (m_states[i].GetLuaState() == L)
        {
            m_states.erase(m_states.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

CLuaInterface& CStateManager::GetInterface(lua_State* L)
{
    for (auto& entry : m_states)
    {
        if (entry.GetLuaState() == L)
            return entry;
    }

    // We couldn't find the state, need to insert a new one
    return SetupState(L);
}

const std::vector<CLuaInterface>& CStateManager::GetStates()
{
    return m_states;
}

// Global state manager
CStateManager GarrysMod::Lua::g_StateManager;
