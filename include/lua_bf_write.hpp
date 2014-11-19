#ifndef LUA_BF_WRITE_HPP
#define LUA_BF_WRITE_HPP

#include "bitbuf.h"
#include "gm/CLuaInterface.hpp"

class lua_bf_write : public bf_write
{
public:
    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    lua_bf_write(int nBytes);
    ~lua_bf_write();

    int LuaReset(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaSeekToBit(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaWriteOneBit(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteUBitLong(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteSBitLong(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteBytes(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteString(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteFloat(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaWriteByte(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteShort(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteWord(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaWriteLong(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaGetNumBytesLeft(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaGetNumBytesWritten(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaGetNumBitsLeft(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaGetNumBitsWritten(GarrysMod::Lua::CLuaInterface& Lua);
    int LuaGetMaxNumBits(GarrysMod::Lua::CLuaInterface& Lua);

    int LuaIsOverflowed(GarrysMod::Lua::CLuaInterface& Lua);

    static int LuaCreateWriter(GarrysMod::Lua::CLuaInterface& Lua);

    static void InitializeMetaFunctions(GarrysMod::Lua::CLuaInterface& Lua, GarrysMod::Lua::CLuaObject& mtIndex);
};

#endif
