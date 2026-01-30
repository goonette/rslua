#pragma once

#include <MinHook.h>

namespace memory {

#define HOOK_FUNCTION(offset, hook, original)                                                                                              \
    {                                                                                                                                      \
        auto* target = reinterpret_cast<LPVOID>(offset);                                                                                   \
        if (MH_CreateHook(target, static_cast<LPVOID>(hook), reinterpret_cast<LPVOID*>(&original)) != MH_OK)                               \
            return FALSE;                                                                                                                  \
        if (MH_EnableHook(target) != MH_OK)                                                                                                \
            return FALSE;                                                                                                                  \
    }

#define UNHOOK_FUNCTION(offset, original)                                                                                                  \
    {                                                                                                                                      \
        auto* target = reinterpret_cast<LPVOID>(offset);                                                                                   \
        if (MH_DisableHook(target) != MH_OK)                                                                                               \
            return FALSE;                                                                                                                  \
        if (MH_RemoveHook(target) != MH_OK)                                                                                                \
            return FALSE;                                                                                                                  \
        (original) = nullptr;                                                                                                              \
    }

bool patch_bytes(void* dst, const void* src, size_t len);

} // namespace memory