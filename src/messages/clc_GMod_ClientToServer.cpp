#include "CGModNetMessage.hpp"
#include "gm/LuaBindThunk.hpp"

using namespace GarrysMod::Lua;

IMPLEMENT_MESSAGE_BASECLASS(clc_GMod_ClientToServer, CGModNetMessage)
{
public:
    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        clc_GMod_ClientToServer* obj = MSG_REGISTRATION(clc_GMod_ClientToServer).CreateNewMessage();
        Lua.PushBoundObject(obj);
        obj->SetDataLua(Lua);
        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetData", LuaMemberBindThunk<clc_GMod_ClientToServer, BaseClass, &GetDataLua>);
        mtIndex.SetMember("SetData", LuaMemberBindThunk<clc_GMod_ClientToServer, BaseClass, &SetDataLua>);
        mtIndex.SetMember("GetNumBits", LuaMemberBindThunk<clc_GMod_ClientToServer, BaseClass, &GetNumBitsLua>);
    }
};

const char* clc_GMod_ClientToServer::LuaMetaTableName = "nethook.clc_GMod_ClientToServer";
const int clc_GMod_ClientToServer::LuaTypeID = 205;
