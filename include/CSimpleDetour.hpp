#ifndef CSIMPLEDETOUR_HPP
#define CSIMPLEDETOUR_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "detours.h"

#include <string>
#include "Util.hpp"
#include "FuncPtr.hpp"

class DetourException : public std::runtime_error
{
public:
    DetourException(const std::string& errorStr)
        : std::runtime_error(errorStr) {}
};

template <class T>
class CSimpleDetour
{
public:
    CSimpleDetour(FuncPtr<T>* old, FuncPtr<T> replacement)
        : m_fnOld(old), m_fnReplacement(replacement) {}

    void Attach()
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach(&(m_fnOld->voidPtr), m_fnReplacement.voidPtr);

        LONG result = DetourTransactionCommit();
        if (result != NO_ERROR)
        {
            throw DetourException(Util::Format("Failed to attach detour (Old = 0x%p, Hook = 0x%p, Result = 0x%X)", m_fnOld->voidPtr, m_fnReplacement.voidPtr, result));
        }

        m_bAttached = true;
    }

    void Detach()
    {
        if (!m_bAttached)
            return;

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourDetach(&(m_fnOld->voidPtr), m_fnReplacement.voidPtr);

        LONG result = DetourTransactionCommit();
        if (result != NO_ERROR)
        {
            throw DetourException(Util::Format("Failed to detach detour (Old = 0x%p, Hook = 0x%p, Result = 0x%X)", m_fnOld->voidPtr, m_fnReplacement.voidPtr, result));
        }

        m_bAttached = false;
    }

private:
    FuncPtr<T>* m_fnOld;
    FuncPtr<T> m_fnReplacement;

    bool m_bAttached;
};

#endif
