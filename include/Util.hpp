#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

namespace Util
{
	std::string Format(const char* fmt, ...);
	std::wstring Format(const wchar_t* fmt, ...);
	std::wstring Widen(const std::string& input);
	std::string Narrow(const std::wstring& input);
	std::string StringToHex(const char* input, size_t len);
}

#endif
