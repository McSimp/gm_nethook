#include "CLuaInterface.hpp"

#include <iostream>
#include <stdarg.h>

using namespace GarrysMod::Lua;

CLuaInterface::CLuaInterface(lua_State* state)
    : m_pState(state),
    m_pLua(state->luabase),
    m_G(state->luabase),
    m_R(state->luabase),
    m_E(state->luabase),
    m_errorNoHalt(state->luabase),
    m_hookCall(state->luabase),
    m_nethookWriteCallback(state->luabase)
{
    // TODO: avoid a shitty constructor for CLuaObject here

    printf("CLuaInterface constructor\n");

    m_pLua->PushSpecial(SPECIAL_GLOB);
    CLuaObject m_G(m_pLua, m_pLua->ReferenceCreate());

    m_pLua->PushSpecial(SPECIAL_REG);
    CLuaObject m_R(m_pLua, m_pLua->ReferenceCreate());

    m_pLua->PushSpecial(SPECIAL_ENV);
    CLuaObject m_E(m_pLua, m_pLua->ReferenceCreate());

    m_errorNoHalt = GetGlobal("ErrorNoHalt");

    m_hookCall = GetGlobal("hook").GetMember("Call"); // TODO: Move this instead of copy

    m_isServer = GetGlobal("SERVER").GetBool();
}

CLuaInterface::~CLuaInterface()
{
}

lua_State* CLuaInterface::GetLuaState()
{
    return m_pState;
}

CLuaObject& CLuaInterface::Global()
{
    return m_G;
}

CLuaObject& CLuaInterface::Registry()
{
    return m_R;
}

CLuaObject& CLuaInterface::Environment()
{
    return m_E;
}

CLuaObject CLuaInterface::GetNewTable()
{
    NewTable();
    return CLuaObject(m_pLua, m_pLua->ReferenceCreate());
}

void CLuaInterface::NewTable()
{
    m_pLua->CreateTable();
}

CLuaObject CLuaInterface::NewTemporaryObject()
{
    m_pLua->PushNil();
    return CLuaObject(m_pLua, m_pLua->ReferenceCreate());
}

CLuaObject CLuaInterface::NewUserData(const CLuaObject& metaT)
{
    UserData* data = (UserData*)m_pLua->NewUserdata(sizeof(UserData));
    CLuaObject obj = CLuaObject(m_pLua, m_pLua->ReferenceCreate());

    obj.Push(); // +1
    metaT.Push(); // +1
    m_pLua->SetMetaTable(-2); // -1
    m_pLua->Pop(); // -1

    return obj;
}

void CLuaInterface::PushUserData(const CLuaObject& metaT, void* v, unsigned char type, bool needsGC)
{
    UserData* data = (UserData*)m_pLua->NewUserdata(sizeof(UserData));
    data->data = v;
    data->type = type;
    data->needsGC = needsGC;

    int iRef = m_pLua->ReferenceCreate();

    m_pLua->ReferencePush(iRef);
    metaT.Push(); // +1
    m_pLua->SetMetaTable(-2); // -1
    m_pLua->ReferenceFree(iRef);
}

void CLuaInterface::Error(const char* strError, ...)
{
    char buff[1024];

    va_list argptr;
    va_start(argptr, strError);
#ifdef _LINUX
    vsprintf(buff, strError, argptr);
#else
    vsprintf_s(buff, strError, argptr);
#endif
    va_end(argptr);

    m_pLua->ThrowError(buff);
}

void CLuaInterface::ErrorNoHalt(const char* strError, ...)
{
    char buff[1024];
    va_list argptr;
    va_start(argptr, strError);
#ifdef _LINUX
    vsprintf(buff, strError, argptr);
#else
    vsprintf_s(buff, strError, argptr);
#endif
    va_end(argptr);

    m_errorNoHalt.Push();
    m_pLua->PushString(strError);
    m_pLua->Call(1, 0);
}

void CLuaInterface::LuaError(const char* strError, int argument)
{
    m_pLua->ArgError(argument, strError);
}

CLuaObject CLuaInterface::GetGlobal(const char* name)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->GetField(-1, name); // +1
    int ref = m_pLua->ReferenceCreate(); // -1
    m_pLua->Pop();
    return CLuaObject(m_pLua, ref);
}

void CLuaInterface::SetGlobal(const char* name, CFunc f)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    m_pLua->PushCFunction(f); // +1
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

void CLuaInterface::SetGlobal(const char* name, double d)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    m_pLua->PushNumber(d); // +1
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

void CLuaInterface::SetGlobal(const char* name, const char* str)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    m_pLua->PushString(str); // +1
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

void CLuaInterface::SetGlobal(const char* name, bool b)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    m_pLua->PushBool(b); // +1
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

void CLuaInterface::SetGlobal(const char* name, void* u)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    m_pLua->PushUserdata(u); // +1
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

void CLuaInterface::SetGlobal(const char* name, const CLuaObject& o)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    o.Push(); // +1
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

void CLuaInterface::RemoveGlobal(const char* name)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    m_pLua->PushNil(); // +1
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

void CLuaInterface::NewGlobalTable(const char* name)
{
    m_pLua->PushSpecial(SPECIAL_GLOB); // +1
    m_pLua->PushString(name); // +1
    m_pLua->CreateTable();
    m_pLua->SetTable(-3); // -2
    m_pLua->Pop(); // -1
}

CLuaObject CLuaInterface::GetObject(int i)
{
    if (i != 0)
        m_pLua->Push(i); // +1
    return CLuaObject(m_pLua, m_pLua->ReferenceCreate()); // -1
}

const char* CLuaInterface::GetString(int i, unsigned int* iLen)
{
    return m_pLua->GetString(i, iLen);
}

int CLuaInterface::GetInteger(int i)
{
    return (int)GetNumber(i);
}

double CLuaInterface::GetNumber(int i)
{
    return m_pLua->GetNumber(i);
}

double CLuaInterface::GetDouble(int i)
{
    return GetNumber(i);
}

float CLuaInterface::GetFloat(int i)
{
    return (float)GetNumber(i);
}

bool CLuaInterface::GetBool(int i)
{
    return m_pLua->GetBool(i);
}

void** CLuaInterface::GetUserDataPtr(int i)
{
    UserData* data = (UserData*)m_pLua->GetUserdata(i);
    return &data->data; // Not sure if this is correct
}

// TODO: Make template
void* CLuaInterface::GetUserData(int i)
{
    UserData* data = (UserData*)m_pLua->GetUserdata(i);
    return data->data;
}

UserData* CLuaInterface::GetRawUserData(int i)
{
    return (UserData*)m_pLua->GetUserdata(i);
}

void* CLuaInterface::CheckAndGetUserData(int i, int type)
{
    if (m_pLua->GetType(1) != type)
    {
        m_pLua->ArgError(i, "invalid userdata type");
    }

    UserData* data = (UserData*)m_pLua->GetUserdata(i);
    return data->data;
}

UserData* CLuaInterface::CheckAndGetRawUserData(int i, int type)
{
    if (m_pLua->GetType(1) != type)
    {
        m_pLua->ArgError(i, "invalid userdata type");
    }

    return (UserData*)m_pLua->GetUserdata(i);
}

void CLuaInterface::GetTable(int i) // ???
{
    Error("CLuaInterface::GetTable( int i ) is not implemented yet, but feel free to contribute!");
}

const char* CLuaInterface::GetStringOrError(int i)
{
    m_pLua->CheckType(i, Type::STRING);
    return m_pLua->GetString(i);
}

int CLuaInterface::GetReference(int i)
{
    if (i != 0)
        m_pLua->Push(i); // +??
    return m_pLua->ReferenceCreate();
}

void CLuaInterface::FreeReference(int i)
{
    m_pLua->ReferenceFree(i);
}

void CLuaInterface::PushReference(int i)
{
    m_pLua->ReferencePush(i);
}

void CLuaInterface::Pop(int i)
{
    m_pLua->Pop(i);
}

int CLuaInterface::Top()
{
    return m_pLua->Top();
}

void CLuaInterface::Push(const CLuaObject& o)
{
    o.Push();
}

void CLuaInterface::Push(const char* str, unsigned int iLen)
{
    m_pLua->PushString(str, iLen);
}

void CLuaInterface::PushVA(const char* str, ...)
{
    char buff[1024];
    va_list argptr;
    va_start(argptr, str);
#ifdef _LINUX
    vsprintf( buff, str, argptr );
#else
    vsprintf_s(buff, str, argptr);
#endif
    va_end(argptr);
    m_pLua->PushString(buff);
}

void CLuaInterface::Push(double d)
{
    m_pLua->PushNumber(d);
}

void CLuaInterface::Push(bool b)
{
    m_pLua->PushBool(b);
}

void CLuaInterface::Push(CFunc f)
{
    m_pLua->PushCFunction(f);
}

void CLuaInterface::Push(int i)
{
    m_pLua->PushNumber(i);
}

void CLuaInterface::Push(float f)
{
    m_pLua->PushNumber(f);
}

void CLuaInterface::PushLong(int i)
{
    m_pLua->PushNumber(i);
}

void CLuaInterface::PushNil()
{
    m_pLua->PushNil();
}

void CLuaInterface::CheckType(int i, int iType)
{
    return m_pLua->CheckType(i, iType);
}

int CLuaInterface::GetType(int iStackPos)
{
    return m_pLua->GetType(iStackPos);
}

const char* CLuaInterface::GetTypeName(int iType)
{
    return m_pLua->GetTypeName(iType);
}

CLuaObject CLuaInterface::GetReturnObject(int iNum)
{
    return GetObject(iNum);
}

void CLuaInterface::Call(int args, int returns)
{
    m_pLua->Call(args, returns);
}

int CLuaInterface::PCall(int args, int returns, int iErrorFunc)
{
    return m_pLua->PCall(args, returns, iErrorFunc);
}

CLuaObject CLuaInterface::GetMetaTable(const char* strName, int iType)
{
    m_pLua->CreateMetaTableType(strName, iType);
    return CLuaObject(m_pLua, m_pLua->ReferenceCreate());
}

CLuaObject CLuaInterface::GetMetaTable(int i)
{
    if (m_pLua->GetMetaTable(i))
        return CLuaObject(m_pLua, m_pLua->ReferenceCreate());
    else
        return CLuaObject(m_pLua); // A nil object
}

const CLuaObject& CLuaInterface::GetHookCall()
{
    return m_hookCall;
}

void CLuaInterface::SetNethookWriteCallback(CLuaObject& obj)
{
    m_nethookWriteCallback = obj;
}

const CLuaObject& CLuaInterface::GetNethookWriteCallback()
{
    return m_nethookWriteCallback;
}

bool CLuaInterface::IsServer()
{
    return m_isServer;
}

bool CLuaInterface::IsClient()
{
    return !m_isServer;
}
