#ifndef GARRYSMOD_LUA_CSTATEMANAGER_HPP
#define GARRYSMOD_LUA_CSTATEMANAGER_HPP

#include <vector>
#include "Lua.hpp"
#include "CLuaInterface.hpp"

namespace GarrysMod
{
	namespace Lua
	{
		class CStateManager
		{
		public:
			CStateManager();
			CLuaInterface& SetupState(lua_State* L);
			void RemoveState(lua_State* L);
			CLuaInterface& GetInterface(lua_State* L);

		private:
			std::vector<CLuaInterface> m_states; // TODO: Profile
		};

		extern CStateManager g_StateManager;
	}
}

#endif
