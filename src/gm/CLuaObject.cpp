#include "CLuaObject.hpp"
#include "UserData.hpp"
#include <sstream>

using namespace GarrysMod::Lua;

CLuaObject::CLuaObject(ILuaBase* lua) : m_pLua(lua), m_iRef(LUA_REFNIL)
{
	printf("[WARN] CLuaObject default constructor called\n");
}

CLuaObject::CLuaObject(ILuaBase* lua, int iRef) : m_pLua(lua), m_iRef(iRef)
{
	printf("[WARN] CLuaObject ref constructor called\n");
}

CLuaObject::CLuaObject(CLuaObject&& old) : m_pLua(old.m_pLua), m_iRef(old.m_iRef)
{
	printf("[WARN] CLuaObject move constructor called\n");

	old.m_iRef = LUA_REFNIL;
}

CLuaObject::CLuaObject(CLuaObject& old)
{
	printf("[WARN] CLuaObject copy constructor called\n");

	m_pLua = old.m_pLua;
	old.Push();
	m_iRef = m_pLua->ReferenceCreate();
}

CLuaObject& CLuaObject::operator=(CLuaObject& old)
{
	printf("[WARN] CLuaObject copy assignment operator called\n");

	if(m_iRef != LUA_REFNIL)
	{
		printf("[WARN] CLuaObject copy operator had to cleanup\n");
		m_pLua->ReferenceFree(m_iRef);
	}

	m_pLua = old.m_pLua;
	old.Push();
	m_iRef = m_pLua->ReferenceCreate();

	return *this;
}

CLuaObject& CLuaObject::operator=(CLuaObject&& old)
{
	printf("[WARN] CLuaObject move assignment operator called\n");

	if(this != &old)
	{
		if(m_iRef != LUA_REFNIL)
		{
			printf("[WARN] CLuaObject move operator had to cleanup\n");
			m_pLua->ReferenceFree(m_iRef);
		}

		m_pLua = old.m_pLua;
		m_iRef = old.m_iRef;

		old.m_iRef = LUA_REFNIL;
	}

	return *this;
}

CLuaObject::~CLuaObject()
{
	printf("[WARN] CLuaObject destructor called\n");

	m_pLua->ReferenceFree(m_iRef);
}

void CLuaObject::Set(const CLuaObject& obj) // ???
{
	if(m_iRef)
		m_pLua->ReferenceFree(m_iRef);

	m_pLua->ReferencePush(obj.m_iRef);
	m_iRef = m_pLua->ReferenceCreate();
}

void CLuaObject::SetNil() // ???
{
	m_pLua->PushNil();
	SetFromStack(-1);
}

void CLuaObject::SetFromStack(int i)
{
	if(m_iRef)
		m_pLua->ReferenceFree(m_iRef);

	m_pLua->Push(i);
	m_iRef = m_pLua->ReferenceCreate();
}

int CLuaObject::GetReference() const
{
	return m_iRef;
}

int CLuaObject::GetType() const
{
	Push(); // +1
		int ret = m_pLua->GetType(-1);
	m_pLua->Pop(); // -1
	return ret;
}

const char* CLuaObject::GetTypeName() const
{
	Push(); // +1
		const char* ret = m_pLua->GetTypeName(m_pLua->GetType(-1));
	m_pLua->Pop(); // -1
	return ret;
}

const char* CLuaObject::GetString(unsigned int* iLen) const
{
	Push(); // +1
		const char* ret = m_pLua->GetString(-1, iLen);
	m_pLua->Pop(); // -1
	return ret;
}

int CLuaObject::GetInt() const
{
	Push(); // +1
		int ret = (int)m_pLua->GetNumber(-1);
	m_pLua->Pop(); // -1
	return ret;
}

double CLuaObject::GetDouble() const
{
	Push(); // +1
		double ret = m_pLua->GetNumber( -1 );
	m_pLua->Pop(); // -1
	return ret;
}

float CLuaObject::GetFloat() const
{
	Push(); // +1
		float ret = (float)m_pLua->GetNumber(-1);
	m_pLua->Pop(); // -1
	return ret;
}

bool CLuaObject::GetBool() const
{
	Push(); // +1
		bool ret = m_pLua->GetBool(-1);
	m_pLua->Pop(); // -1
	return ret;
}

void* CLuaObject::GetUserData() const
{
	Push(); // +1
		UserData* data = (UserData*)m_pLua->GetUserdata(-1);
	m_pLua->Pop(); // -1
	return data->data;
}

void CLuaObject::SetMember(const char* name)
{
	Push(); // +1
		m_pLua->PushString(name); // +1
		m_pLua->PushNil(); // +1
		m_pLua->SetTable(-3); // +2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(const char* name, const CLuaObject& obj)
{
	Push(); // +1
		m_pLua->PushString(name); // +1
		obj.Push(); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(const char* name, double d)
{
	Push(); // +1
		m_pLua->PushString(name); // +1
		m_pLua->PushNumber(d); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(const char* name, bool b)
{
	Push(); // +1
		m_pLua->PushString(name); // +1
		m_pLua->PushBool(b); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(const char* name, const char* s, unsigned int iLen)
{
	Push(); // +1
		m_pLua->PushString(name); // +1
		m_pLua->PushString(s, iLen); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(const char* name, CFunc f)
{
	Push(); // +1
		m_pLua->PushString(name); // +1
		m_pLua->PushCFunction(f); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(double dKey)
{
	Push(); // +1
		m_pLua->PushNumber(dKey); // +1
		m_pLua->PushNil(); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(double dKey, const CLuaObject& obj)
{
	Push(); // +1
		m_pLua->PushNumber(dKey); // +1
		obj.Push(); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(double dKey, double d)
{
	Push(); // +1
		m_pLua->PushNumber(dKey); // +1
		m_pLua->PushNumber(d); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(double dKey, bool b)
{
	Push(); // +1
		m_pLua->PushNumber(dKey); // +1
		m_pLua->PushBool(b); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(double dKey, const char* s, unsigned int iLen)
{
	Push(); // +1
		m_pLua->PushNumber(dKey); // +1
		m_pLua->PushString(s, iLen); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(double dKey, CFunc f)
{
	Push(); // +1
		m_pLua->PushNumber(dKey); // +1
		m_pLua->PushCFunction(f); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMember(float fKey)
{
	SetMember((double)fKey);
}

void CLuaObject::SetMember(float fKey, const CLuaObject& obj)
{
	SetMember((double)fKey, obj);
}

void CLuaObject::SetMember(float fKey, double d)
{
	SetMember((double)fKey, d);
}

void CLuaObject::SetMember(float fKey, bool b)
{
	SetMember((double)fKey, b);
}

void CLuaObject::SetMember(float fKey, const char* s, unsigned int iLen)
{
	SetMember((double)fKey, s, iLen);
}

void CLuaObject::SetMember(float fKey, CFunc f)
{
	SetMember((double)fKey, f);
}

void CLuaObject::SetMember(int iKey)
{
	SetMember((double)iKey);
}

void CLuaObject::SetMember(int iKey, const CLuaObject& obj)
{
	SetMember((double)iKey, obj);
}

void CLuaObject::SetMember(int iKey, double d)
{
	SetMember((double)iKey, d);
}

void CLuaObject::SetMember(int iKey, bool b)
{
	SetMember((double)iKey, b);
}

void CLuaObject::SetMember(int iKey, const char* s, unsigned int iLen)
{
	SetMember((double)iKey, s, iLen);
}

void CLuaObject::SetMember(int iKey, CFunc f)
{
	SetMember((double)iKey, f);
}

CLuaObject CLuaObject::GetMember(const char* name) const
{
	Push(); // +1
		m_pLua->GetField(-1, name); // +1
		int ref = m_pLua->ReferenceCreate(); // -1
	m_pLua->Pop(); // -1

	return CLuaObject(m_pLua, ref);
}

CLuaObject CLuaObject::GetMember(double dKey) const
{
	Push(); // +1
		m_pLua->PushNumber(dKey); // +1
		m_pLua->GetTable(-2);
		int ref = m_pLua->ReferenceCreate(); // -1
	m_pLua->Pop(); // -1

	return CLuaObject(m_pLua, ref);
}

CLuaObject CLuaObject::GetMember(float fKey) const
{
	return GetMember((double)fKey);
}

CLuaObject CLuaObject::GetMember(int iKey) const
{
	return GetMember((double)iKey);
}

CLuaObject CLuaObject::GetMember(const CLuaObject& oKey) const
{
	Push(); // +1
		oKey.Push(); // +1
		m_pLua->GetTable(-2); // -2 AND +1
		int ref = m_pLua->ReferenceCreate();
	m_pLua->Pop(); // -1

	return CLuaObject(m_pLua, ref);
}

bool CLuaObject::GetMemberBool(const char* name, bool b) const
{
	Push(); // +1
		m_pLua->GetField(-1, name); // +1
		bool r = (m_pLua->GetType(-1) != Type::NIL) ? m_pLua->GetBool(-1) : b;
	m_pLua->Pop(2); // -2
	return r;
}

int CLuaObject::GetMemberInt(const char* name, int i) const
{
	return (int)GetMemberDouble(name, (int)i);
}

float CLuaObject::GetMemberFloat(const char* name, float f) const
{
	return (float)GetMemberDouble(name, (double) f);
}

double CLuaObject::GetMemberDouble(const char* name, double d) const
{
	Push(); // +1
		m_pLua->GetField(-1, name); // +1
		double r = (m_pLua->GetType(-1) != Type::NIL) ? m_pLua->GetNumber(-1) : d;
	m_pLua->Pop(2); // -2
	return r;
}

const char* CLuaObject::GetMemberStr(const char* name, const char* s, unsigned int* iLen) const
{
	Push(); // +1
		m_pLua->GetField(-1, name); // +1
		const char* r = (m_pLua->GetType(-1) != Type::NIL) ? m_pLua->GetString(-1, iLen) : s;
	m_pLua->Pop(2); // -2
	return r;
}

void* CLuaObject::GetMemberUserData(const char* name, void* u) const
{
	Push(); // +1
		m_pLua->GetField(-1, name); // +1
		void* r = (m_pLua->GetType(-1) != Type::NIL) ? ((UserData*)m_pLua->GetUserdata(-1))->data : u;
	m_pLua->Pop(2); // -2
	return r;
}

void CLuaObject::SetUserData(void* obj, unsigned char type)
{
	Push(); // +1
		UserData *data = (UserData*)m_pLua->GetUserdata();
		data->data = obj;
		data->type = type;
	m_pLua->Pop(); // -1
}

void CLuaObject::SetMemberUserDataLite(const char* name, void* pData)
{
	Push(); // +1
		m_pLua->PushString(name); // +1
		m_pLua->PushUserdata(pData); // +1
		m_pLua->SetTable(-3); // -2
	m_pLua->Pop(); // -1
}

void* CLuaObject::GetMemberUserDataLite(const char* name, void* u) const
{
	Push(); // +1
		m_pLua->GetField(-1, name); // +1
		void* r  = (m_pLua->GetType(-1) != Type::NIL) ? m_pLua->GetUserdata(-1) : u;
	m_pLua->Pop(2); // -2
	return r;
}

bool CLuaObject::IsType(int iType) const
{
	Push(); // +1
		bool ret = m_pLua->IsType(-1, iType);
	m_pLua->Pop(); // -1
	return ret;
}

bool CLuaObject::IsNil() const
{
	return IsType(Type::NIL);
}

bool CLuaObject::IsTable() const
{
	return IsType(Type::TABLE);
}

bool CLuaObject::IsString() const
{
	return IsType(Type::STRING);
}

bool CLuaObject::IsNumber() const
{
	return IsType(Type::NUMBER);
}

bool CLuaObject::IsFunction() const
{
	return IsType(Type::FUNCTION);
}

bool CLuaObject::IsUserData() const
{
	Push(); // +1
		int iType = m_pLua->GetType(-1);
	m_pLua->Pop(); // -1

	return iType == Type::USERDATA || iType > Type::COUNT;
}

void CLuaObject::Push() const
{
	m_pLua->ReferencePush(m_iRef);
}
