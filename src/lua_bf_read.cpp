#include "lua_bf_read.hpp"
#include "gm/LuaBindThunk.hpp"

using namespace GarrysMod::Lua;

lua_bf_read::lua_bf_read(const unsigned char* oldBuf, int writeSize)
{
    unsigned char* buf = new unsigned char[writeSize];
    memcpy(buf, oldBuf, writeSize);
    StartReading(buf, writeSize);
}

lua_bf_read::~lua_bf_read()
{
    delete[] m_pData;
}

int lua_bf_read::LuaReset(CLuaInterface& Lua)
{
    Reset();
    return 0;
}

int lua_bf_read::LuaReadOneBit(CLuaInterface& Lua)
{
    Lua.Push(ReadOneBit());
    return 1;
}

int lua_bf_read::LuaGetNumBytesLeft(CLuaInterface& Lua)
{
    Lua.Push(GetNumBytesLeft());
    return 1;
}

int lua_bf_read::LuaGetNumBytesRead(CLuaInterface& Lua)
{
    Lua.Push(GetNumBytesRead());
    return 1;
}

int lua_bf_read::LuaGetNumBitsLeft(CLuaInterface& Lua)
{
    Lua.Push(GetNumBitsLeft());
    return 1;
}

int lua_bf_read::LuaGetNumBitsRead(CLuaInterface& Lua)
{
    Lua.Push(GetNumBitsRead());
    return 1;
}

int lua_bf_read::LuaSeek(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    Lua.Push(Seek(Lua.GetInteger(2)));
    return 1;
}

int lua_bf_read::LuaSeekRelative(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    Lua.Push(SeekRelative(Lua.GetInteger(2)));
    return 1;
}

int lua_bf_read::LuaReadByte(CLuaInterface& Lua)
{
    Lua.Push(ReadByte());
    return 1;
}

int lua_bf_read::LuaReadWord(CLuaInterface& Lua)
{
    Lua.Push(ReadWord());
    return 1;
}

int lua_bf_read::LuaReadUBitLong(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    Lua.Push(ReadUBitLong(Lua.GetInteger(2)));
    return 1;
}

int lua_bf_read::LuaReadSBitLong(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    Lua.Push(ReadSBitLong(Lua.GetInteger(2)));
    return 1;
}

int lua_bf_read::LuaReadUInt(CLuaInterface& Lua)
{
    return LuaReadUBitLong(Lua);
}

int lua_bf_read::LuaReadInt(CLuaInterface& Lua)
{
    return LuaReadSBitLong(Lua);
}

int lua_bf_read::LuaReadFloat(CLuaInterface& Lua)
{
    Lua.Push(ReadFloat());
    return 1;
}

int lua_bf_read::LuaReadLong(CLuaInterface& Lua)
{
    Lua.Push(ReadLong());
    return 1;
}

int lua_bf_read::LuaReadString(CLuaInterface& Lua)
{
    static char strBuf[102400];
    int remaining = 0;
    
    if (!ReadString(strBuf, sizeof(strBuf), false, &remaining))
    {
        Lua.Error("The requested string was too large to read");
    }
    else
    {
        if (strlen(strBuf) != (sizeof(strBuf)-remaining - 1))
        {
            Lua.Error("Sizes do not match: %d %d", strlen(strBuf), (sizeof(strBuf)-remaining - 1));
        }

        Lua.Push(strBuf, sizeof(strBuf) - remaining - 1);
    }

    return 1;
}

int lua_bf_read::LuaReadBytes(CLuaInterface& Lua)
{
    static char buf[102400];

    Lua.CheckType(2, Type::NUMBER);
    int numBytes = Lua.GetInteger(2);
    
    if (numBytes > sizeof(buf))
    {
        Lua.Error("Cannot read %d bytes, maximum is %d", numBytes, sizeof(buf));
    }
    else if (numBytes <= 0)
    {
        Lua.Push("");
    } 
    else if (!ReadBytes(buf, numBytes))
    {
        Lua.Error("Failed to read bytes");
    }
    else
    {
        Lua.Push(buf, numBytes);
    }

    return 1;
}

int lua_bf_read::LuaIsOverflowed(CLuaInterface& Lua)
{
    Lua.Push(IsOverflowed());
    return 1;
}

void lua_bf_read::InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
{
    mtIndex.SetMember("Reset", LuaMemberBindThunk<lua_bf_read, &LuaReset>);
    mtIndex.SetMember("ReadOneBit", LuaMemberBindThunk<lua_bf_read, &LuaReadOneBit>);
    mtIndex.SetMember("GetNumBytesLeft", LuaMemberBindThunk<lua_bf_read, &LuaGetNumBytesLeft>);
    mtIndex.SetMember("GetNumBytesRead", LuaMemberBindThunk<lua_bf_read, &LuaGetNumBytesRead>);
    mtIndex.SetMember("GetNumBitsLeft", LuaMemberBindThunk<lua_bf_read, &LuaGetNumBitsLeft>);
    mtIndex.SetMember("GetNumBitsRead", LuaMemberBindThunk<lua_bf_read, &LuaGetNumBitsRead>);
    mtIndex.SetMember("Seek", LuaMemberBindThunk<lua_bf_read, &LuaSeek>);
    mtIndex.SetMember("SeekRelative", LuaMemberBindThunk<lua_bf_read, &LuaSeekRelative>);
    mtIndex.SetMember("ReadByte", LuaMemberBindThunk<lua_bf_read, &LuaReadByte>);
    mtIndex.SetMember("ReadWord", LuaMemberBindThunk<lua_bf_read, &LuaReadWord>);
    mtIndex.SetMember("ReadUBitLong", LuaMemberBindThunk<lua_bf_read, &LuaReadUBitLong>);
    mtIndex.SetMember("ReadSBitLong", LuaMemberBindThunk<lua_bf_read, &LuaReadSBitLong>);
    mtIndex.SetMember("ReadUInt", LuaMemberBindThunk<lua_bf_read, &LuaReadUInt>);
    mtIndex.SetMember("ReadInt", LuaMemberBindThunk<lua_bf_read, &LuaReadInt>);
    mtIndex.SetMember("ReadLong", LuaMemberBindThunk<lua_bf_read, &LuaReadLong>);
    mtIndex.SetMember("ReadFloat", LuaMemberBindThunk<lua_bf_read, &LuaReadFloat>);
    mtIndex.SetMember("ReadString", LuaMemberBindThunk<lua_bf_read, &LuaReadString>);
    mtIndex.SetMember("ReadBytes", LuaMemberBindThunk<lua_bf_read, &LuaReadBytes>);
    mtIndex.SetMember("IsOverflowed", LuaMemberBindThunk<lua_bf_read, &LuaIsOverflowed>);
}

const char* lua_bf_read::LuaMetaTableName = "nethook.bf_read";
const int lua_bf_read::LuaTypeID = 198;
