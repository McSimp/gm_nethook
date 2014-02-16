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

    static void InitializeLua(GarrysMod::Lua::CLuaInterface& Lua);
    static int GetPlayerNetChannelLua(GarrysMod::Lua::CLuaInterface& Lua);
};

#endif
