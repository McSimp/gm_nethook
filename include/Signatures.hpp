#ifndef SIGNATURES_HPP
#define SIGNATURES_HPP

#include "MemorySignature.hpp"

namespace Signatures
{
    MemorySignature CBaseClientState_ConnectionStart = {
        "\x55\x8B\xEC\x51\x53\x56\x57\x8B\xF9\x6A\x1C",
        "xxxxxxxxxxx",
        11
    };

    MemorySignature CBaseClient_ConnectionStart = {
        "\x55\x8B\xEC\x51\x53\x56\x57\x8B\xF1\x6A\x1C",
        "xxxxxxxxxxx",
        11
    };

    MemorySignature svc_CreateStringTableFunc = {
        "\x56\x8B\xF1\x8D\x4E\x30",
        "xxxxxx",
        6
    };
}

#endif
