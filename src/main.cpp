#include <memory.hpp>
#include <offsets.hpp>

#include <windows.h>

#include <print>

static uintptr_t module_base_g = []() -> uintptr_t {
    uintptr_t peb = __readgsqword(0x60);
    return *reinterpret_cast<uintptr_t*>(peb + 0x10);
}();

static double print_client_address() {
    const uintptr_t client = *reinterpret_cast<uintptr_t*>(module_base_g + offsets::CLIENT_INSTANCE);
    if (client) {
        std::println("client: 0x{:X}", client);
        return 0;
    } else {
        return 1;
    }
}

using register_double_t = double(__fastcall*)(uintptr_t, void*, const char*);
static register_double_t register_double_o = nullptr;

static double __fastcall register_double_h(uintptr_t state, void* callback, const char* name) {
    register_double_o(state, print_client_address, "printClientAddress");

    return register_double_o(state, callback, name);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        AllocConsole();
        const auto _ = freopen_s((FILE**)stdout, "conout$", "w", stdout);

        const byte patch[] = {0xB0, 0x01, 0xC3};
        memory::patch_bytes(reinterpret_cast<void*>(module_base_g + offsets::PATCH_CHECK_FOR_LUA), &patch, sizeof(patch));

        if (MH_Initialize() != MH_OK)
            return FALSE;

        HOOK_FUNCTION(module_base_g + offsets::LUA_REGISTER_NUMBER, register_double_h, register_double_o);
    } else if (reason == DLL_PROCESS_DETACH) {
        MH_Uninitialize();
    }

    return TRUE;
}
