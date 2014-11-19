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
    int         m_nClientCRC; // Probably m_nClientCRC, always -1 though
    char        m_mapMD5[16];    //  32
    int         m_nMaxClients;   //  48
    int         m_nMaxClasses;   //  52
    int         m_nPlayerSlot;   //  56
    float       m_fTickInterval; //  60
    const char  *m_szGameDir;    //  64
    const char  *m_szMapName;    //  68
    const char  *m_szSkyName;    //  72
    const char  *m_szHostName;   //  76
    const char  *m_szLoadingURL;  //  80
    const char  *m_szGamemode;   //  84

    char        m_szGameDirBuffer[260];
    char        m_szMapNameBuffer[260];
    char        m_szSkyNameBuffer[260];
    char        m_szHostNameBuffer[260];
    char        m_szLoadingURLBuffer[260];
    char        m_szGamemodeBuffer[260];

    static const char* LuaMetaTableName;
    static const int LuaTypeID;

    int GetProtocolLua(CLuaInterface& Lua)
    {
        Lua.Push(m_nProtocol);
        return 1;
    }

    int SetProtocolLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_nProtocol = Lua.GetInteger(2);
        return 0;
    }

    int GetServerCountLua(CLuaInterface& Lua)
    {
        Lua.Push(m_nServerCount);
        return 1;
    }

    int SetServerCountLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_nServerCount = Lua.GetInteger(2);
        return 0;
    }

    int GetIsDedicatedLua(CLuaInterface& Lua)
    {
        Lua.Push(m_bIsDedicated);
        return 1;
    }

    int SetIsDedicatedLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::BOOL);
        m_bIsDedicated = Lua.GetBool(2);
        return 0;
    }

    int GetIsHLTVLua(CLuaInterface& Lua)
    {
        Lua.Push(m_bIsHLTV);
        return 1;
    }

    int SetIsHLTVLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::BOOL);
        m_bIsHLTV = Lua.GetBool(2);
        return 0;
    }

    int GetOSLua(CLuaInterface& Lua)
    {
        Lua.Push(&m_cOS, 1);
        return 1;
    }

    int SetOSLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newOS = Lua.GetString(2, &len);

        if (len != 1)
        {
            Lua.LuaError("OS must be 1 character", 2);
        }

        m_cOS = newOS[0];

        return 0;
    }

    int GetClientCRCLua(CLuaInterface& Lua)
    {
        Lua.Push(m_nClientCRC);
        return 1;
    }

    int SetClientCRCLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_nClientCRC = Lua.GetInteger(2);
        return 0;
    }

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

    int GetMaxClientsLua(CLuaInterface& Lua)
    {
        Lua.Push(m_nMaxClients);
        return 1;
    }

    int SetMaxClientsLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_nMaxClients = Lua.GetInteger(2);
        return 0;
    }

    int GetMaxClassesLua(CLuaInterface& Lua)
    {
        Lua.Push(m_nMaxClasses);
        return 1;
    }

    int SetMaxClassesLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_nMaxClasses = Lua.GetInteger(2);
        return 0;
    }

    int GetPlayerSlotLua(CLuaInterface& Lua)
    {
        Lua.Push(m_nPlayerSlot);
        return 1;
    }

    int SetPlayerSlotLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_nPlayerSlot = Lua.GetInteger(2);
        return 0;
    }

    int GetTickIntervalLua(CLuaInterface& Lua)
    {
        Lua.Push(m_fTickInterval);
        return 1;
    }

    int SetTickIntervalLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::NUMBER);
        m_fTickInterval = Lua.GetFloat(2);
        return 0;
    }

    int GetGameDirLua(CLuaInterface& Lua)
    {
        Lua.Push(m_szGameDir);
        return 1;
    }

    int SetGameDirLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newValue = Lua.GetString(2, &len);

        if (len >= sizeof(m_szGameDirBuffer))
        {
            Lua.LuaError("Length of value must be less than 260", 2);
        }

        V_strcpy_safe(m_szGameDirBuffer, newValue);
        m_szGameDir = m_szGameDirBuffer;

        return 0;
    }

    int GetMapNameLua(CLuaInterface& Lua)
    {
        Lua.Push(m_szMapName);
        return 1;
    }

    int SetMapNameLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newValue = Lua.GetString(2, &len);

        if (len >= sizeof(m_szMapNameBuffer))
        {
            Lua.LuaError("Length of value must be less than 260", 2);
        }

        V_strcpy_safe(m_szMapNameBuffer, newValue);
        m_szMapName = m_szMapNameBuffer;

        return 0;
    }

    int GetSkyNameLua(CLuaInterface& Lua)
    {
        Lua.Push(m_szSkyName);
        return 1;
    }

    int SetSkyNameLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newValue = Lua.GetString(2, &len);

        if (len >= sizeof(m_szSkyNameBuffer))
        {
            Lua.LuaError("Length of value must be less than 260", 2);
        }

        V_strcpy_safe(m_szSkyNameBuffer, newValue);
        m_szSkyName = m_szSkyNameBuffer;

        return 0;
    }

    int GetHostNameLua(CLuaInterface& Lua)
    {
        Lua.Push(m_szHostName);
        return 1;
    }

    int SetHostNameLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newValue = Lua.GetString(2, &len);

        if (len >= sizeof(m_szHostNameBuffer))
        {
            Lua.LuaError("Length of value must be less than 260", 2);
        }

        V_strcpy_safe(m_szHostNameBuffer, newValue);
        m_szHostName = m_szHostNameBuffer;

        return 0;
    }

    int GetLoadingURLLua(CLuaInterface& Lua)
    {
        Lua.Push(m_szLoadingURL);
        return 1;
    }

    int SetLoadingURLLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newValue = Lua.GetString(2, &len);

        if (len >= sizeof(m_szLoadingURLBuffer))
        {
            Lua.LuaError("Length of value must be less than 260", 2);
        }

        V_strcpy_safe(m_szLoadingURLBuffer, newValue);
        m_szLoadingURL = m_szLoadingURLBuffer;

        return 0;
    }

    int GetGamemodeLua(CLuaInterface& Lua)
    {
        Lua.Push(m_szGamemode);
        return 1;
    }

    int SetGamemodeLua(CLuaInterface& Lua)
    {
        Lua.CheckType(2, Type::STRING);

        unsigned int len = 0;
        const char* newValue = Lua.GetString(2, &len);

        if (len >= sizeof(m_szGamemodeBuffer))
        {
            Lua.LuaError("Length of value must be less than 260", 2);
        }

        V_strcpy_safe(m_szGamemodeBuffer, newValue);
        m_szGamemode = m_szGamemodeBuffer;

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
        mtIndex.SetMember("GetProtocol", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetProtocolLua>);
        mtIndex.SetMember("SetProtocol", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetProtocolLua>);
        mtIndex.SetMember("GetServerCount", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetServerCountLua>);
        mtIndex.SetMember("SetServerCount", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetServerCountLua>);
        mtIndex.SetMember("GetIsDedicated", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetIsDedicatedLua>);
        mtIndex.SetMember("SetIsDedicated", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetIsDedicatedLua>);
        mtIndex.SetMember("GetIsHLTV", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetIsHLTVLua>);
        mtIndex.SetMember("SetIsHLTV", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetIsHLTVLua>);
        mtIndex.SetMember("GetOS", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetOSLua>);
        mtIndex.SetMember("SetOS", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetOSLua>);
        mtIndex.SetMember("GetClientCRC", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetClientCRCLua>);
        mtIndex.SetMember("SetClientCRC", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetClientCRCLua>);
        mtIndex.SetMember("GetMapMD5", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetMapMD5Lua>);
        mtIndex.SetMember("SetMapMD5", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetMapMD5Lua>);
        mtIndex.SetMember("GetMaxClients", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetMaxClientsLua>);
        mtIndex.SetMember("SetMaxClients", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetMaxClientsLua>);
        mtIndex.SetMember("GetMaxClasses", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetMaxClassesLua>);
        mtIndex.SetMember("SetMaxClasses", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetMaxClassesLua>);
        mtIndex.SetMember("GetPlayerSlot", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetPlayerSlotLua>);
        mtIndex.SetMember("SetPlayerSlot", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetPlayerSlotLua>);
        mtIndex.SetMember("GetTickInterval", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetTickIntervalLua>);
        mtIndex.SetMember("SetTickInterval", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetTickIntervalLua>);
        mtIndex.SetMember("GetGameDir", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetGameDirLua>);
        mtIndex.SetMember("SetGameDir", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetGameDirLua>);
        mtIndex.SetMember("GetMapName", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetMapNameLua>);
        mtIndex.SetMember("SetMapName", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetMapNameLua>);
        mtIndex.SetMember("GetSkyName", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetSkyNameLua>);
        mtIndex.SetMember("SetSkyName", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetSkyNameLua>);
        mtIndex.SetMember("GetHostName", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetHostNameLua>);
        mtIndex.SetMember("SetHostName", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetHostNameLua>);
        mtIndex.SetMember("GetLoadingURL", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetLoadingURLLua>);
        mtIndex.SetMember("SetLoadingURL", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetLoadingURLLua>);
        mtIndex.SetMember("GetGamemode", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::GetGamemodeLua>);
        mtIndex.SetMember("SetGamemode", LuaMemberBindThunk<svc_ServerInfo, &svc_ServerInfo::SetGamemodeLua>);
    }
};

const char* svc_ServerInfo::LuaMetaTableName = "nethook.svc_ServerInfo";
const int svc_ServerInfo::LuaTypeID = 202;
