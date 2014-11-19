#include "lua_bf_write.hpp"
#include "gm/LuaBindThunk.hpp"

using namespace GarrysMod::Lua;

lua_bf_write::lua_bf_write(int nBytes)
{
    unsigned char* buf = new unsigned char[nBytes];
    StartWriting(buf, nBytes);
}

lua_bf_write::~lua_bf_write()
{
    delete[] m_pData;
}

int lua_bf_write::LuaReset(CLuaInterface& Lua)
{
    Reset();
    return 0;
}

int lua_bf_write::LuaSeekToBit(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    SeekToBit(Lua.GetInteger(2));
    return 0;
}

int lua_bf_write::LuaWriteOneBit(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::BOOL);
    WriteOneBit(Lua.GetBool(2));
    return 0;
}

int lua_bf_write::LuaWriteUBitLong(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    Lua.CheckType(3, Type::NUMBER);

    WriteUBitLong(Lua.GetInteger(2), Lua.GetInteger(3));
    return 0;
}

int lua_bf_write::LuaWriteSBitLong(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    Lua.CheckType(3, Type::NUMBER);

    WriteSBitLong(Lua.GetInteger(2), Lua.GetInteger(3));
    return 0;
}

int lua_bf_write::LuaWriteBytes(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::STRING);
    Lua.CheckType(3, Type::NUMBER);

    unsigned int dataLen;
    const char* data = Lua.GetString(2, &dataLen);
    unsigned int numBytes = Lua.GetInteger(3);

    if (numBytes > dataLen)
    {
        Lua.Error("Cannot write %d bytes, data is not long enough", numBytes);
    }
    else if (numBytes <= 0)
    {
        Lua.LuaError("Data length must be greater than 0", 3);
    }
    else if (!WriteBytes(data, numBytes))
    {
        Lua.Error("Failed to write data");
    }

    return 0;
}

int lua_bf_write::LuaWriteString(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::STRING);

    if (!WriteString(Lua.GetString(2)))
    {
        Lua.Error("The string was too large to write");
    }

    return 0;
}

int lua_bf_write::LuaWriteFloat(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    WriteFloat(Lua.GetNumber());
    return 0;
}

int lua_bf_write::LuaWriteByte(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    WriteByte(Lua.GetInteger(2));
    return 0;
}

int lua_bf_write::LuaWriteShort(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    WriteShort(Lua.GetInteger(2));
    return 0;
}

int lua_bf_write::LuaWriteWord(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    WriteWord(Lua.GetInteger(2));
    return 0;
}

int lua_bf_write::LuaWriteLong(CLuaInterface& Lua)
{
    Lua.CheckType(2, Type::NUMBER);
    WriteLong(Lua.GetNumber(2));
    return 0;
}

int lua_bf_write::LuaGetNumBytesLeft(CLuaInterface& Lua)
{
    Lua.Push(GetNumBytesLeft());
    return 1;
}

int lua_bf_write::LuaGetNumBytesWritten(CLuaInterface& Lua)
{
    Lua.Push(GetNumBytesWritten());
    return 1;
}

int lua_bf_write::LuaGetNumBitsLeft(CLuaInterface& Lua)
{
    Lua.Push(GetNumBitsLeft());
    return 1;
}

int lua_bf_write::LuaGetNumBitsWritten(CLuaInterface& Lua)
{
    Lua.Push(GetNumBitsWritten());
    return 1;
}

int lua_bf_write::LuaGetMaxNumBits(CLuaInterface& Lua)
{
    Lua.Push(GetMaxNumBits());
    return 1;
}

int lua_bf_write::LuaIsOverflowed(CLuaInterface& Lua)
{
    Lua.Push(IsOverflowed());
    return 1;
}

int lua_bf_write::LuaCreateWriter(CLuaInterface& Lua)
{
    Lua.CheckType(1, Type::NUMBER);
    int size = Lua.GetInteger(1);

    lua_bf_write* writer = new lua_bf_write(size);
    Lua.PushBoundObject(writer);

    return 1;
}

void lua_bf_write::InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
{
    mtIndex.SetMember("Reset", LuaMemberBindThunk<lua_bf_write, &LuaReset>);
    mtIndex.SetMember("SeekToBit", LuaMemberBindThunk<lua_bf_write, &LuaSeekToBit>);
    mtIndex.SetMember("WriteOneBit", LuaMemberBindThunk<lua_bf_write, &LuaWriteOneBit>);
    mtIndex.SetMember("WriteUBitLong", LuaMemberBindThunk<lua_bf_write, &LuaWriteUBitLong>);
    mtIndex.SetMember("WriteSBitLong", LuaMemberBindThunk<lua_bf_write, &LuaWriteSBitLong>);
    mtIndex.SetMember("WriteBytes", LuaMemberBindThunk<lua_bf_write, &LuaWriteBytes>);
    mtIndex.SetMember("WriteString", LuaMemberBindThunk<lua_bf_write, &LuaWriteString>);
    mtIndex.SetMember("WriteFloat", LuaMemberBindThunk<lua_bf_write, &LuaWriteFloat>);
    mtIndex.SetMember("WriteByte", LuaMemberBindThunk<lua_bf_write, &LuaWriteByte>);
    mtIndex.SetMember("WriteShort", LuaMemberBindThunk<lua_bf_write, &LuaWriteShort>);
    mtIndex.SetMember("WriteWord", LuaMemberBindThunk<lua_bf_write, &LuaWriteWord>);
    mtIndex.SetMember("WriteLong", LuaMemberBindThunk<lua_bf_write, &LuaWriteLong>);
    mtIndex.SetMember("GetNumBytesLeft", LuaMemberBindThunk<lua_bf_write, &LuaGetNumBytesLeft>);
    mtIndex.SetMember("GetNumBytesWritten", LuaMemberBindThunk<lua_bf_write, &LuaGetNumBytesWritten>);
    mtIndex.SetMember("GetNumBitsLeft", LuaMemberBindThunk<lua_bf_write, &LuaGetNumBitsLeft>);
    mtIndex.SetMember("GetNumBitsWritten", LuaMemberBindThunk<lua_bf_write, &LuaGetNumBitsWritten>);
    mtIndex.SetMember("GetMaxNumBits", LuaMemberBindThunk<lua_bf_write, &LuaGetMaxNumBits>);
    mtIndex.SetMember("IsOverflowed", LuaMemberBindThunk<lua_bf_write, &LuaIsOverflowed>);

    CLuaObject nethookTbl = Lua.GetGlobal("nethook");
    nethookTbl.SetMember("bf_write", LuaStaticBindThunk<&LuaCreateWriter>);
}

const char* lua_bf_write::LuaMetaTableName = "nethook.bf_write";
const int lua_bf_write::LuaTypeID = 197;
