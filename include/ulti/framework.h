#ifndef SHAREDMEMORYIPC_ULTI_FRAMEWORK_H_
#define SHAREDMEMORYIPC_ULTI_FRAMEWORK_H_

#include <string>
#include <iostream>
#include <filesystem>

#include <stdio.h>
#include <string.h>

#include "ulti/define.h"

namespace ulti
{
    void ZeroMemory(void* data, DWORD size);
}

#endif