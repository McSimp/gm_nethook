#ifndef CNETCHANNEL_HPP
#define CNETCHANNEL_HPP

#include "inetchannel.h"
#include "gm/CLuaInterface.hpp"

class CNetChannel : public INetChannel
{
public:
    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    int SendNetMsgLua(GarrysMod::Lua::CLuaInterface& Lua);
    int RequestFileLua(GarrysMod::Lua::CLuaInterface& Lua);

    static void InitializeMetaFunctions(GarrysMod::Lua::CLuaInterface& Lua, GarrysMod::Lua::CLuaObject& mtIndex);
    static int GetPlayerNetChannelLua(GarrysMod::Lua::CLuaInterface& Lua);
};

#endif
