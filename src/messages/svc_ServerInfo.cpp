#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"
#include "gm/LuaBindThunk.hpp"
#include "CHookedNetMessage.hpp"
#include "strtools.h"

using namespace GarrysMod::Lua;

class svc_ServerInfo;
CMessageClassRegistration<svc_ServerInfo> Registration("svc_ServerInfo");

class svc_ServerInfo : public CHookedNetMessage<svc_ServerInfo, Registration>
{
public:
    int         m_nProtocol;
    int         m_nServerCount;
    bool        m_bIsDedicated;
    bool        m_bIsHLTV;
    char        m_cOS;
    int         Unknown;
    char        m_mapMD5[16];    //  32
    int         m_nMaxClients;   //  48
    int         m_nMaxClasses;   //  52
    int         m_nPlayerSlot;   //  56
    float       m_fTickInterval; //  60
    const char  *m_szGameDir;    //  64
    const char  *m_szMapName;    //  68
    const char  *m_szSkyName;    //  72
    const char  *m_szHostName;   //  76
    const char  *m_sLoadingURL;  //  80
    const char  *m_szGamemode;   //  84

    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    int GetMapMD5Lua(CLuaInterface& Lua)
    {
        Lua.Push(m_mapMD5, 16);
        return 1;
    }

    int SetMapMD5Lua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newMD5 = Lua.GetString(2, &len);

        if (len != 16)
        {
            Lua.LuaError("MD5 hash must be of length 16", 2);
        }

        memcpy(m_mapMD5, newMD5, 16);

        return 0;
    }

    static int LuaCreateObject(CLuaInterface& Lua)
    {
        svc_ServerInfo* obj = Registration.CreateNewMessage();
        Lua.PushBoundObject(obj);

        return 1;
    }

    static void InitializeMetaFunctions(CLuaInterface& Lua, CLuaObject& mtIndex)
    {
        mtIndex.SetMember("GetMapMD5", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetMapMD5Lua>);
        mtIndex.SetMember("SetMapMD5", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetMapMD5Lua>);
    }
};

const char* svc_ServerInfo::LuaMetaTableName = "nethook.svc_ServerInfo";
const int svc_ServerInfo::LuaTypeID = 202;
