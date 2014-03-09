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

                virtual void Unknown1() = 0;
                virtual void Unknown2() = 0;
                virtual void Unknown3() = 0;
                virtual void Unknown4() = 0;
                virtual void Unknown5() = 0;
                virtual void Unknown6() = 0;
                virtual void Unknown7() = 0;
                virtual void Unknown8() = 0;
                virtual void Unknown9() = 0;
                virtual void Unknown10() = 0;
                virtual void Unknown11() = 0;
                virtual void Unknown12() = 0;
                virtual void Unknown13() = 0;
                virtual bool CallInternal(int numArgs, int numReturns) = 0;
                virtual void Unknown15() = 0;
                virtual bool CallInternalGetBool(int numArgs) = 0;
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

