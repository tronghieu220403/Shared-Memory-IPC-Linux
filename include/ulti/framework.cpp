#include "framework.h"

namespace ulti
{
    void ZeroMemory(void* data, DWORD size)
    {
        ::memset(data, 0, size);
    }
    void PrintDebug(const std::string_view& str)
    {
        std::cout << str << std::endl;
    }

}
