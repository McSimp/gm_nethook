#include "CNetMessageRegistration.hpp"
#include "CNetMessage.hpp"
#include "gm/Lua.hpp"

using namespace GarrysMod::Lua;

class SVC_Print;
CTemplatedRegistration<SVC_Print> Registration("svc_Print");

class SVC_Print : public CNetMessage
{
	SVC_Print() { m_bReliable = false; m_text = NULL; };

public:
	void* m_pMessageHandler;
	const char* m_text;

	bool WriteHook(bf_write& buffer)
	{
		Msg("Hi there everything!\n");
		return Registration.CallOriginalWrite(this, buffer);
	}

	static void InitializeLua(CLuaInterface& Lua)
	{
		Msg("svc_Print is being added to Lua!\n");
		Registration.Attach();
	}
};
