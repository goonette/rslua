#include <memory.hpp>

#include <cstring>
#include <windows.h>

namespace memory {

bool patch_bytes(void *dst, const void *src, size_t len) {
    DWORD oldProt = 0;
    if (!VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &oldProt))
        return false;

    std::memcpy(dst, src, len);
    FlushInstructionCache(GetCurrentProcess(), dst, len);

    DWORD dummy = 0;
    VirtualProtect(dst, len, oldProt, &dummy);
    return true;
}

} // namespace memory