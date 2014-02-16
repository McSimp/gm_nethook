#ifndef NETMESSAGEMANAGER_HPP
#define NETMESSAGEMANAGER_HPP

#include "CNetMessageRegistration.hpp"
#include "gm/CLuaInterface.hpp"

class NetMessageResolverException : public std::runtime_error
{
public:
    NetMessageResolverException(const std::string& errorStr)
        : std::runtime_error(errorStr) {}
};

namespace NetMessageManager
{
    void ResolveMessages();
    void AddMessagesToLua(GarrysMod::Lua::CLuaInterface& Lua);
    void UnhookAllMessages();
    bool AttachMessage(const std::string& messageName);
    bool DetachMessage(const std::string& messageName);
    int SetWriteCallbackLua(GarrysMod::Lua::CLuaInterface& Lua);
    int AttachMessageLua(GarrysMod::Lua::CLuaInterface& Lua);
    int DetachMessageLua(GarrysMod::Lua::CLuaInterface& Lua);
}

#endif
