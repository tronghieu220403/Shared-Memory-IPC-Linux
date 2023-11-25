#include "framework.h"

namespace ulti
{
    void ZeroMemory(void* data, DWORD size)
    {
        ::memset(data, 0, size);
    }
}
