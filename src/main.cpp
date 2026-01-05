#include <memory.hpp>
#include <offsets.hpp>

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        const uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA(NULL));

        const byte patch[] = {0xB0, 0x01, 0xC3};
        memory::patch_bytes(reinterpret_cast<void*>(base + offsets::PATCH_CHECK_FOR_LUA), &patch,
                            sizeof(patch));
    }

    return TRUE;
}
