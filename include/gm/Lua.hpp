#ifndef GARRYSMOD_LUA_HPP
#define GARRYSMOD_LUA_HPP

#include "Types.hpp"
#include "LuaBase.hpp"
#include "UserData.hpp"

struct lua_State
{
	unsigned char				_ignore_this_common_lua_header_[69];
	GarrysMod::Lua::ILuaBase*	luabase;
};

#ifdef _WIN32
	#define DLL_EXPORT extern "C" __declspec( dllexport )
#else
	#define DLL_EXPORT extern "C" __attribute__( ( visibility("default") ) )	
#endif

#define GMOD_MODULE_OPEN()	DLL_EXPORT int gmod13_open(lua_State* L)
#define GMOD_MODULE_CLOSE()	DLL_EXPORT int gmod13_close(lua_State* L)

#define LUA_FUNCTION( _function_ ) int _function_(lua_State* L)
#define UsesLua()	GarrysMod::Lua::CLuaInterface& Lua = GarrysMod::Lua::g_StateManager.GetInterface(L)

#endif 
