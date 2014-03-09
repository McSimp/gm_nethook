#ifndef LUA_BF_READ_HPP
#define LUA_BF_READ_HPP

#include "bitbuf.h"
#include "gm/CLuaInterface.hpp"

class lua_bf_read : public bf_read
{
public:
    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    lua_bf_read(const bf_write& write) : lua_bf_read(write.GetData(), write.m_nDataBytes) {};
    lua_bf_read(const bf_write& write, int numBits) : lua_bf_read(write.GetData(), (numBits + 7) / 8) {};
    lua_bf_read(const unsigned char* oldBuf, int writeSize);
    ~lua_bf_read();

    int LuaReset(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaReadOneBit(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaGetNumBytesLeft(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaGetNumBytesRead(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaGetNumBitsLeft(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaGetNumBitsRead(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaSeek(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaSeekRelative(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaReadByte(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaReadWord(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaReadUBitLong(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaReadSBitLong(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaReadUInt(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaReadInt(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaReadLong(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaReadFloat(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaReadString(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaReadBytes(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaIsOverflowed(GarrysMod::Lua::CLuaInterface& Lua);

    static void InitializeMetaFunctions(GarrysMod::Lua::CLuaInterface& Lua, GarrysMod::Lua::CLuaObject& mtIndex);
};

#endif
