#include "NetMessageManager.hpp"
#include "CSigScan.hpp"
#include "Signatures.hpp"
#include "Util.hpp"
#include "CNetMessage.hpp"
#include <map>

using namespace GarrysMod::Lua;

namespace NetMessageManager
{
	void ProcessVTable(void** vtable)
	{
		// Caution: the following line may cause you some concern.
		INetMessage* msg = (INetMessage*)&vtable;

		// TODO: Check that the vtable address is somewhat sane - maybe check
		// that it's a valid address in the module's address space?

		const char* messageName = msg->GetName();
#ifdef _DEBUG
		Msg("[nethook] Found Message: %s\n", messageName);
#endif

		auto& messages = CNetMessageRegistration::RegisteredMessages;

		auto it = messages.find(messageName);
		if (it != messages.end())
		{
			CNetMessageRegistration& msg = it->second;
			if (!msg.IsResolved())
			{
				msg.ResolveFromVTable(vtable);
			}
		}
	}

	void ResolveMessagesFromFunctionCode(unsigned char* funcCode)
	{
		// Function ends with CC as alignment
		while (*(int*)funcCode != 0xCCCCCCCC)
		{
			unsigned char current = *funcCode++;
			unsigned char next = *funcCode;

			// If this is a mov dword ptr [edi], offset xxx or mov dword ptr [eax], offset xxx
			// or mov dword ptr [esi], offset xxx
			if (current == 0xC7 && (next == 0x00 || next == 0x07 || next == 0x06))
			{
				funcCode++;
				void** vtable = *(void***)funcCode;
				funcCode += 4;
				ProcessVTable(vtable);
			}
		}
	}

	void ResolveMessages()
	{
		// This function is super hacky - but it's the best way I can come up with to avoid either
		// a) Waiting around to capture and instance of every message or
		// b) Sigscanning 20-odd signatures just to get what we need.

		// Basically, we grab the code from CBaseClientState::ConnectionStart and from
		// CBaseClient::ConnectionStart and look for the instructions that move the vtable pointer
		// into net message objects. Then, call the GetName function in each vtable to find which
		// message its associated with, then pass the vtable on to the correct CNetMsgRegistration
		// so we can resolve the original WriteToBuffer and Process functions. After we know the
		// locations of these functions, we can detour them if needed.

		// I don't just overwrite the pointers in the vtable since WriteToBuffer is often called
		// "statically" (ie. not through the vtable), so we have to detour the actual function itself.

		CSigScan engineScan("engine.dll");

		unsigned char* clientStateConnectionStartCode = static_cast<unsigned char*>(engineScan.Scan(Signatures::CBaseClientState_ConnectionStart));
		ResolveMessagesFromFunctionCode(clientStateConnectionStartCode);

		unsigned char* clientConnectionStartCode = static_cast<unsigned char*>(engineScan.Scan(Signatures::CBaseClient_ConnectionStart));
		ResolveMessagesFromFunctionCode(clientConnectionStartCode);

		// svc_CreateStringTable has its own function for some stupid reason.
		unsigned char* createStringTableCode = static_cast<unsigned char*>(engineScan.Scan(Signatures::svc_CreateStringTableFunc));
		ResolveMessagesFromFunctionCode(createStringTableCode);

		// Check that all the messages have been setup properly. If they haven't, then gmod has probably been updated.
		const auto& messages = CNetMessageRegistration::RegisteredMessages;
		for (const auto& registration : messages)
		{
			if (!registration.second.IsResolved())
				throw NetMessageResolverException(Util::Format("Failed to find net message: %s", registration.first.c_str()));
		}
	}

	void AddMessagesToLua(CLuaInterface& Lua)
	{
		const auto& messages = CNetMessageRegistration::RegisteredMessages;
		for (const auto& registration : messages)
		{
			registration.second.InitializeLua(Lua);
		}
	}

	void UnhookAllMessages()
	{
		auto& messages = CNetMessageRegistration::RegisteredMessages;
		for (auto& registration : messages)
		{
			registration.second.Detach();
		}
	}
}
