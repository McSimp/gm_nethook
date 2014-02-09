#ifndef GARRYSMOD_LUA_USERDATA_HPP
#define GARRYSMOD_LUA_USERDATA_HPP

namespace GarrysMod 
{
	namespace Lua
	{
		struct UserData
		{
			void*			data;
			unsigned char	type;
		};
	}
}

#endif 
