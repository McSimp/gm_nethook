#include "CGModNetMessage.hpp"
#include "gm/LuaBindThunk.hpp"

using namespace GarrysMod::Lua;

IMPLEMENT_MESSAGE_BASECLASS(svc_GMod_ServerToClient, CGModNetMessage)
{
public:
    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        svc_GMod_ServerToClient* obj = MSG_REGISTRATION(svc_GMod_ServerToClient).CreateNewMessage();
        Lua.PushBoundObject(obj);
        obj->SetDataLua(Lua);
        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetData", LuaMemberBindThunk<svc_GMod_ServerToClient, BaseClass, &GetDataLua>);
        mtIndex.SetMember("SetData", LuaMemberBindThunk<svc_GMod_ServerToClient, BaseClass, &SetDataLua>);
        mtIndex.SetMember("GetNumBits", LuaMemberBindThunk<svc_GMod_ServerToClient, BaseClass, &GetNumBitsLua>);
    }
};

const char* svc_GMod_ServerToClient::LuaMetaTableName = "nethook.svc_GMod_ServerToClient";
const int svc_GMod_ServerToClient::LuaTypeID = 201;
