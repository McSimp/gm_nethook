#ifndef GARRYSMOD_LUA_CLUAOBJECT_HPP
#define GARRYSMOD_LUA_CLUAOBJECT_HPP

#include "Lua.hpp"

namespace GarrysMod
{
	namespace Lua
	{
		class CLuaObject
		{
		public:
			CLuaObject(ILuaBase* lua);
			CLuaObject(ILuaBase* lua, int iRef);
			CLuaObject(CLuaObject& old);
			CLuaObject(CLuaObject&& old);
			CLuaObject& operator=(CLuaObject& old);
			CLuaObject& operator=(CLuaObject&& old);
			~CLuaObject();

			void			Set(const CLuaObject& obj);
			void			SetNil();
			void			SetFromStack(int i);

			void			UnReference();
			int				GetReference() const;

			int				GetType() const;
			const char*		GetTypeName() const;

			const char*		GetString(unsigned int* iLen = nullptr) const;
			int				GetInt() const;
			double			GetDouble() const;
			float			GetFloat() const;
			bool			GetBool() const;
			void*			GetUserData() const;

			// CUtlLuaVector*	GetMembers() const; TODO

			void			SetMember(const char* name);
			void			SetMember(const char* name, const CLuaObject& obj);
			void			SetMember(const char* name, double d);
			void			SetMember(const char* name, bool b);
			void			SetMember(const char* name, const char* s, unsigned int iLen = 0);
			void			SetMember(const char* name, CFunc f);

			void			SetMember(double dKey);
			void			SetMember(double dKey, const CLuaObject& obj);
			void			SetMember(double dKey, double d);
			void			SetMember(double dKey, bool b);
			void			SetMember(double dKey, const char* s, unsigned int iLen = 0);
			void			SetMember(double dKey, CFunc f);

			void			SetMember(float fKey);
			void			SetMember(float fKey, const CLuaObject& obj);
			void			SetMember(float fKey, double d);
			void			SetMember(float fKey, bool b);
			void			SetMember(float fKey, const char* s, unsigned int iLen = 0);
			void			SetMember(float fKey, CFunc f);

			void			SetMember(int iKey);
			void			SetMember(int iKey, const CLuaObject& obj);
			void			SetMember(int iKey, double d);
			void			SetMember(int iKey, bool b);
			void			SetMember(int iKey, const char* s, unsigned int iLen = 0);
			void			SetMember(int iKey, CFunc f);

			CLuaObject		GetMember(const char* name) const;
			CLuaObject		GetMember(double dKey) const;
			CLuaObject		GetMember(float fKey) const;
			CLuaObject		GetMember(int iKey) const;
			CLuaObject		GetMember(const CLuaObject& oKey) const;

			bool			GetMemberBool(const char* name, bool b = true) const;
			int				GetMemberInt(const char* name, int i = 0) const;
			float			GetMemberFloat(const char* name, float f = 0.0f) const;
			double			GetMemberDouble(const char* name, double d = 0) const;
			const char*		GetMemberStr(const char* name, const char* s = "", unsigned int* iLen = nullptr) const;
			void*			GetMemberUserData(const char* name, void* = nullptr) const;

			void			SetMemberUserDataLite(const char* name, void* pData);
			void*			GetMemberUserDataLite(const char* name, void* u = nullptr) const;

			void			SetUserData(void* obj, unsigned char type);

			bool			IsType(int iType) const;
			bool			IsNil() const;
			bool			IsTable() const;
			bool			IsString() const;
			bool			IsNumber() const;
			bool			IsFunction() const;
			bool			IsUserData() const;

			void			Push() const;

		private:
			ILuaBase* m_pLua;
			int m_iRef;
		};
	}
}
#endif