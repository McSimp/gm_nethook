#ifndef CVIRTUALHOOK_HPP
#define CVIRTUALHOOK_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include "Util.hpp"
#include "FuncPtr.hpp"

class VirtualHookException : public std::runtime_error
{
public:
    VirtualHookException(const std::string& errorStr)
        : std::runtime_error(errorStr) {}
};

template <class T>
class CVirtualHook
{
public:
    CVirtualHook(void** vtable, int index, FuncPtr<T> replacement)
        : m_vtable(vtable),
        m_index(index), 
        m_fnReplacement(replacement)
    {
        
    }

private:
    void** m_vtable;
    int m_index;
    FuncPtr<T> m_fnReplacement;
    FuncPtr<T> m_fnOld;

    bool m_bAttached;
};

#endif
