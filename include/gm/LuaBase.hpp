#ifndef GARRYSMOD_LUA_LUABASE_HPP
#define GARRYSMOD_LUA_LUABASE_HPP

#include <stddef.h>

struct lua_State;

namespace GarrysMod 
{
	namespace Lua
	{
		typedef int (*CFunc) (lua_State *L);
		const int LUA_REFNIL = -1;

		//
		// Access to raw Lua function calls
		//
		class ILuaBase
		{
			public:

				virtual int			Top( void ) = 0;
				virtual void		Push( int iStackPos ) = 0;
				virtual void		Pop( int iAmt = 1 ) = 0;
				virtual void		GetTable( int iStackPos ) = 0;
				virtual void		GetField( int iStackPos, const char* strName ) = 0;
				virtual void		SetField( int iStackPos, const char* strName ) = 0;
				virtual void		CreateTable() = 0;
				virtual void		SetTable( int i ) = 0;
				virtual void		SetMetaTable( int i ) = 0;
				virtual bool		GetMetaTable( int i ) = 0;
				virtual void		Call( int iArgs, int iResults ) = 0;
				virtual int			PCall( int iArgs, int iResults, int iErrorFunc ) = 0;
				virtual int			Equal( int iA, int iB ) = 0;
				virtual int			RawEqual( int iA, int iB ) = 0;
				virtual void		Insert( int iStackPos ) = 0;
				virtual void		Remove( int iStackPos ) = 0;
				virtual int			Next( int iStackPos ) = 0;
				virtual void*		NewUserdata( unsigned int iSize ) = 0;
				virtual void		ThrowError( const char* strError ) = 0;
				virtual void		CheckType( int iStackPos, int iType ) = 0;
				virtual void		ArgError( int iArgNum, const char* strMessage ) = 0;
				virtual void		RawGet( int iStackPos ) = 0;
				virtual void		RawSet( int iStackPos ) = 0;

				virtual const char*		GetString( int iStackPos = -1, unsigned int* iOutLen = nullptr ) = 0;
				virtual double			GetNumber( int iStackPos = -1 ) = 0;
				virtual bool			GetBool( int iStackPos = -1 ) = 0;
				virtual CFunc			GetCFunction( int iStackPos = -1 ) = 0;
				virtual void*			GetUserdata( int iStackPos = -1 ) = 0;

				virtual void		PushNil() = 0;
				virtual void		PushString( const char* val, unsigned int iLen = 0 ) = 0;
				virtual void		PushNumber( double val ) = 0;
				virtual void		PushBool( bool val ) = 0;
				virtual void		PushCFunction( CFunc val ) = 0;
				virtual void		PushCClosure( CFunc val, int iVars ) = 0;
				virtual void		PushUserdata( void* ) = 0;

				//
				// If you create a reference - don't forget to free it!
				//
				virtual int			ReferenceCreate() = 0;
				virtual void		ReferenceFree( int i ) = 0;
				virtual void		ReferencePush( int i ) = 0;

				//
				// Push a special value onto the top of the stack ( see below )
				//
				virtual void		PushSpecial( int iType ) = 0;

				//
				// For type enums see Types.h 
				//
				virtual bool			IsType( int iStackPos, int iType ) = 0;
				virtual int				GetType( int iStackPos ) = 0;
				virtual const char*		GetTypeName( int iType ) = 0;

				//
				// Creates a new meta table of string and type and leaves it on the stack.
				// Will return the old meta table of this name if it already exists.
				//
				virtual void			CreateMetaTableType( const char* strName, int iType ) = 0;

				//
				// Like Get* but throws errors and returns if they're not of the expected type
				//
				virtual const char*		CheckString( int iStackPos = -1 ) = 0;
				virtual double			CheckNumber( int iStackPos = -1 ) = 0;

				virtual double			ObjLen( int iStackPos = -1 ) = 0;
                
                // Private functions

                virtual void Unknown00() = 0; // CLuaInterface::AddThreadedCall(GarrysMod::Lua::IThreadedCall *);
                virtual void Unknown01() = 0; // CLuaInterface::Init(ILuaCallback *,bool);
                virtual void Unknown02() = 0; // CLuaInterface::Shutdown(void);
                virtual void Unknown03() = 0; // CLuaInterface::Cycle(void);
                virtual void Unknown04() = 0; // CLuaInterface::GetLuaState(void);
                virtual void Unknown05() = 0; // CLuaInterface::Global(void);
                virtual void Unknown06() = 0; // CLuaInterface::GetObject(int);
                virtual void Unknown07() = 0; // CLuaInterface::DELETE_ME_5437(int);
                virtual void Unknown08() = 0; // CLuaInterface::_DELETE_ME2466(int);
                virtual void Unknown09() = 0; // CLuaInterface::PushLuaObject(ILuaObject *);
                virtual void Unknown10() = 0; // CLuaInterface::PushLuaFunction(int (*)(lua_State *));
                virtual void LuaError(const char*, int argument = -1) = 0; // CLuaInterface::LuaError(char  const*,int);
                virtual void TypeError(const char* name, int argument) = 0; // CLuaInterface::TypeError(char  const*,int);
                virtual bool CallInternal(int numArgs, int numReturns) = 0; // CLuaInterface::CallInternal(int,int);
                virtual bool CallInternalNoReturns(int numArgs) = 0; // CLuaInterface::CallInternalNoReturns(int);
                virtual bool CallInternalGetBool(int numArgs) = 0; // CLuaInterface::CallInternalGetBool(int);
                virtual bool CallInternalGetString(int numArgs) = 0; // CLuaInterface::CallInternalGetString(int);
                virtual void Unknown13() = 0; // CLuaInterface::CallInternalGet(int,ILuaObject *);
                virtual void Unknown14() = 0; // CLuaInterface::_DELETE_ME(ILuaObject *,void *);
                virtual void NewGlobalTable(const char* name); // CLuaInterface::NewGlobalTable(char  const*);
                virtual void Unknown15() = 0; // CLuaInterface::NewTemporaryObject(void);
                virtual bool IsUserData(int iStackPos = -1) = 0; // CLuaInterface::isUserData(int);
                virtual void Unknown16() = 0; // CLuaInterface::GetMetaTableObject(char  const*,int);
                virtual void Unknown17() = 0; // CLuaInterface::GetMetaTableObject(int);
                virtual void Unknown18() = 0; // CLuaInterface::GetReturn(int);
                virtual bool IsServer() = 0; // CLuaInterface::IsServer(void);
                virtual bool IsClient() = 0; // CLuaInterface::IsClient(void);
                virtual bool IsDedicatedServer() = 0; // CLuaInterface::IsDedicatedServer(void);
                virtual void Unknown19() = 0; // CLuaInterface::DestroyObject(ILuaObject *);
                virtual void Unknown20() = 0; // CLuaInterface::CreateObject(void);
                virtual void Unknown21() = 0; // CLuaInterface::SetMember(ILuaObject *,ILuaObject *,ILuaObject *);
                virtual void Unknown22() = 0; // CLuaInterface::GetNewTable(void);
                virtual void Unknown23() = 0; // CLuaInterface::SetMember(ILuaObject *, float);
                virtual void Unknown24() = 0; // CLuaInterface::SetMember(ILuaObject *, float, ILuaObject *);
                virtual void Unknown25() = 0; // CLuaInterface::SetMember(ILuaObject *, char  const*);
                virtual void Unknown26() = 0; // CLuaInterface::SetMember(ILuaObject *, char  const*, ILuaObject *);
                virtual void Unknown27() = 0; // CLuaInterface::SetIsServer(bool);
                virtual void PushLong(long f) = 0; // CLuaInterface::PushLong(long);
                virtual void GetFlags(int iStackPos) = 0; // CLuaInterface::GetFlags(int);
                virtual bool FindOnObjectsMetaTable(int iObject, int iKey) = 0; // CLuaInterface::FindOnObjectsMetaTable(int, int);
                virtual bool FindObjectOnTable(int iTable, int iKey) = 0; // CLuaInterface::FindObjectOnTable(int, int);
                virtual void Unknown28() = 0; // CLuaInterface::SetMemberFast(ILuaObject *, int, int);
                virtual bool RunString(const char* strFilename, const char* strPath, const char* strStringToRun, bool bRun, bool bShowErrors) = 0; // CLuaInterface::RunString(char  const*, char  const*, char  const*, bool, bool);
                virtual void Unknown29() = 0; // CLuaInterface::IsEqual(ILuaObject *, ILuaObject *);
                virtual void Error(const char* strError) = 0; // CLuaInterface::Error(char  const*);
                virtual const char* GetStringOrError(int i) = 0; // CLuaInterface::GetStringOrError(int);
                virtual bool RunLuaModule(const char* strName) = 0; // CLuaInterface::RunLuaModule(char  const*);
                virtual bool FindAndRunScript(const char* strFilename, bool bRun, bool bReportErrors, const char* unknown) = 0; // CLuaInterface::FindAndRunScript(char  const*, bool, bool, char  const*);
                virtual void SetPathID(const char*) = 0; // CLuaInterface::SetPathID(char  const*);
                virtual const char* GetPathID() = 0; // CLuaInterface::GetPathID(void);
                virtual void ErrorNoHalt(const char* fmt, ...) = 0; // CLuaInterface::ErrorNoHalt(char  const*, ...);
                virtual void Msg(const char* fmt, ...) = 0; // CLuaInterface::Msg(char  const*, ...);
                virtual void PushPath(const char* strPath) = 0; // CLuaInterface::PushPath(char  const*);
                virtual void PopPath() = 0; // CLuaInterface::PopPath(void);
                virtual const char* GetPath() = 0; // CLuaInterface::GetPath(void);
                virtual int GetColor(int iStackPos) = 0; // CLuaInterface::GetColor(int);
                virtual void PushColor(int r, int g, int b, int a) = 0; // CLuaInterface::PushColor(int, int, int, int);
                virtual void Unknown30() = 0; // CLuaInterface::GetStack(int level, lua_Debug *ar);
                virtual void Unknown31() = 0; // CLuaInterface::GetInfo(const char *what, lua_Debug *ar);
                virtual void Unknown32() = 0; // CLuaInterface::GetLocal(lua_Debug *ar, int n);
                virtual void Unknown33() = 0; // CLuaInterface::GetUpvalue(int funcindex, int n);
                virtual void Unknown34() = 0; // CLuaInterface::RunStringEx(char  const*, char  const*, char  const*, bool, bool, bool);
                virtual void Unknown35() = 0; // CLuaInterface::DELETE_ME1(char  const*, int);
                virtual int GetDataString(int index, const char** str); // CLuaInterface::GetDataString(int, void **);
                virtual void Unknown36() = 0; // CLuaInterface::ErrorFromLua(char  const*, ...);
                virtual const char* GetCurrentLocation() = 0; // CLuaInterface::GetCurrentLocation(void);
                virtual void Unknown37() = 0; // CLuaInterface::MsgColour(Color  const&, char  const*, ...);
                virtual void SetState(lua_State* state) = 0; // CLuaInterface::SetState(lua_State *);
                virtual void Unknown38() = 0; // ILuaInterface_DeleteMe::DELETE_ME2(void);
                virtual void Unknown39() = 0; // CLuaInterface::GetCurrentFile(std::string &);
                virtual void Unknown40() = 0; // CLuaInterface::CompileString(Bootil::Buffer &, std::string  const&);
                virtual void Unknown41() = 0; // CBaseLuaInterface::ThreadLock(void);
                virtual void Unknown42() = 0; // CBaseLuaInterface::ThreadUnlock(void);
                virtual void Unknown43() = 0; // CLuaInterface::CallFunctionProtected(int, int, bool);
                virtual void Unknown44() = 0; // CLuaInterface::Require(char  const*);
                virtual const char* GetActualTypeName(int tp) = 0; // CLuaInterface::GetActualTypeName(int);
                virtual void Unknown45() = 0; // CLuaInterface::SetupInfiniteLoopProtection(void);
		};

		enum 
		{
			SPECIAL_GLOB,		// Global table
			SPECIAL_ENV,		// Environment table
			SPECIAL_REG,		// Registry table
		};
	}
}

#endif 

