#undef GetObject

#ifndef GARRYSMOD_LUA_CLUAINTERFACE_HPP
#define GARRYSMOD_LUA_CLUAINTERFACE_HPP

#include "CLuaObject.hpp"
#include "Lua.hpp"
#include "dbg.h"

namespace GarrysMod
{
    namespace Lua
    {
        class CLuaInterface
        {
        public:
            CLuaInterface(lua_State* state);
            ~CLuaInterface();

            lua_State*		GetLuaState();

            CLuaObject&		Global();
            CLuaObject&		Registry();
            CLuaObject&		Environment();

            CLuaObject		GetNewTable();
            void			NewTable();
            CLuaObject		NewTemporaryObject();
            CLuaObject		NewUserData(const CLuaObject& metaT);

            void			PushUserData(const CLuaObject& metatable, void * v, unsigned char type, bool needsGC = true);

            void			Error(const char* strError, ...);
            void			ErrorNoHalt(const char* strError, ...);
            void			LuaError(const char* strError, int argument = -1);

            CLuaObject		GetGlobal(const char* name);

            void			SetGlobal(const char* name, CFunc f);
            void			SetGlobal(const char* name, double d);
            void			SetGlobal(const char* name, const char* s);
            void			SetGlobal(const char* name, bool b);
            void			SetGlobal(const char* name, void* u);
            void			SetGlobal(const char* name, const CLuaObject& o);

            void			RemoveGlobal(const char* name);
            void			NewGlobalTable(const char* name);

            CLuaObject		GetObject(int i = -1);
            const char*		GetString(int i = -1, unsigned int* iLen = nullptr);
            int				GetInteger(int i = -1);
            double			GetNumber(int i = -1);
            double			GetDouble(int i = -1);
            float			GetFloat(int i = -1);
            bool			GetBool(int i = -1);
            void**			GetUserDataPtr(int i = -1);

            void*	        GetUserData(int i = -1);
            UserData*       GetRawUserData(int i = -1);
            void*           CheckAndGetUserData(int i, int type);
            UserData*       CheckAndGetRawUserData(int i, int type);

            void			GetTable(int i = -1);
            const char*		GetStringOrError(int i);

            // CUtlLuaVector*	GetAllTableMembers(int iTable); TODO
            // void			DeleteLuaVector(CUtlLuaVector* pVector); TODO

            int				GetReference(int i = -1);
            void			FreeReference(int i);
            void			PushReference(int i);

            void			Pop(int i = 1);
            int				Top();

            void			Push(const CLuaObject& o);
            void			Push(const char* str, unsigned int iLen = 0);
            void			PushVA(const char* str, ...);
            void			Push(double d);
            void			Push(bool b);
            void			Push(CFunc f);
            void			Push(int i);
            void            Push(unsigned int i);
            void			Push(float f);
            void			PushLong(int i);
            void			PushNil();

            void			CheckType(int i, int iType);
            int				GetType(int iStackPos);
            const char*		GetTypeName(int iType); // Only works for builtin Garry's Mod types

            CLuaObject		GetReturnObject(int iNum);

            void			Call(int args, int returns = 0);
            bool            CallGetBool(int args);
            int				PCall(int args, int returns = 0, int iErrorFunc = 0);

            CLuaObject		GetMetaTable(const char* strName, int iType);
            CLuaObject		GetMetaTable(int i);

            const CLuaObject&  GetHookCall();
            void               SetNethookWriteCallback(CLuaObject& obj);
            const CLuaObject&  GetNethookWriteCallback();
            void               SetNethookProcessCallback(CLuaObject& obj);
            const CLuaObject&  GetNethookProcessCallback();

            // Functions for interacting with bound classes
            template <class T>
            void PushBoundObject(T* obj, bool needsGC = true)
            {
                if (obj == nullptr)
                {
                    PushNil();
                }
                else
                {
                    CLuaObject mt = GetMetaTable(T::LuaMetaTableName, T::LuaTypeID);
                    PushUserData(mt, obj, T::LuaTypeID, needsGC);
                }
            }

            template <class T>
            static int UserDataGC(CLuaInterface& Lua) // TODO: Passing CLuaInterface is smelly
            {
                UserData* ud = Lua.CheckAndGetRawUserData(1, T::LuaTypeID);

#ifdef _DEBUG
                Msg("GC: type = %s, needsGC = %d\n", T::LuaMetaTableName, ud->needsGC);
#endif

                if (ud->needsGC)
                {
                    T* obj = static_cast<T*>(ud->data);
                    delete obj;
                    ud->data = nullptr;
                }

                return 0;
            }

            template <class T>
            void InitializeClass()
            {
                CLuaObject mt = GetMetaTable(T::LuaMetaTableName, T::LuaTypeID);
                CLuaObject __index = GetNewTable();
                T::InitializeMetaFunctions(*this, __index);
                mt.SetMember("__index", __index);
                mt.SetMember("__gc", LuaStaticBindThunk<UserDataGC<T>>);
            }

            bool IsClient();
            bool IsServer();

        private:
            lua_State*		m_pState;
            ILuaBase*		m_pLua;
            CLuaObject		m_G;
            CLuaObject		m_R;
            CLuaObject		m_E;
            CLuaObject		m_hookCall;
            CLuaObject      m_nethookWriteCallback;
            CLuaObject      m_nethookProcessCallback;
        };
    }
}
#endif
