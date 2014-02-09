#include "CSigScan.hpp"
#include "Util.hpp"

// Based off CSigScan from https://wiki.alliedmods.net/Signature_Scanning

CSigScan::CSigScan(const std::string& moduleName)
{
	m_moduleHandle = GetModuleHandle(moduleName.c_str());
	if(m_moduleHandle == nullptr)
	{
		throw SigScanException(Util::Format("GetModuleHandle returned NULL (Win32 Error = %d)", GetLastError()));
	}

	MEMORY_BASIC_INFORMATION mem;

	if(!VirtualQuery(m_moduleHandle, &mem, sizeof(mem)))
	{
		throw SigScanException(Util::Format("VirtualQuery returned NULL (Win32 Error = %d)", GetLastError()));
	}

	m_moduleBase = (char*)mem.AllocationBase;
	if(m_moduleBase == nullptr)
	{
		throw SigScanException("mem.AllocationBase was NULL");
	}

	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)mem.AllocationBase;
	IMAGE_NT_HEADERS* pe = (IMAGE_NT_HEADERS*)((unsigned long)dos + (unsigned long)dos->e_lfanew);

	if(pe->Signature != IMAGE_NT_SIGNATURE)
	{
		throw SigScanException("PE signature is not a valid NT signature");
	}

	m_moduleLen = pe->OptionalHeader.SizeOfImage;
}

void* CSigScan::Scan(const MemorySignature& sigStruct)
{
	return Scan(sigStruct.Sig, sigStruct.Mask, sigStruct.Length);
}

void* CSigScan::Scan(const char* sig, const char* mask)
{
	int sigLength = strlen(mask);
	return Scan(sig, mask, sigLength);
}

void* CSigScan::Scan(const char* sig, const char* mask, int sigLength)
{
	char* pData = m_moduleBase;
	char* pEnd = m_moduleBase + m_moduleLen;

	while(pData < pEnd)
	{
		int i;
		for(i = 0; i < sigLength; i++)
		{
			if(mask[i] != '?' && sig[i] != pData[i])
				break;
		}

		// The for loop finished on its own accord
		if(i == sigLength)
		{
			return (void*)pData;
		}

		pData++;
	}

	throw SigScanException(Util::Format("Signature could not be resolved - %s", Util::StringToHex(sig, sigLength).c_str()));
}
