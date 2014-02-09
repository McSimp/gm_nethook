#ifndef SIGSCAN_HPP
#define SIGSCAN_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

#include "MemorySignature.hpp"

class SigScanException : public std::runtime_error
{
public:
	SigScanException(const std::string& errorStr)
		: std::runtime_error(errorStr)
	{
	}
};

class CSigScan
{
private:
	char* m_moduleBase;
	size_t m_moduleLen;
	HMODULE m_moduleHandle;
	
public:
	CSigScan(const std::string& moduleName);

	void* Scan(const MemorySignature& sigStruct);
	void* Scan(const char* sig, const char* mask, int sigLength);
	void* Scan(const char* sig, const char* mask);
};

#endif