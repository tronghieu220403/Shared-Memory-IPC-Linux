#ifndef SHAREDMEMORYIPC_ULTI_FRAMEWORK_H_
#define SHAREDMEMORYIPC_ULTI_FRAMEWORK_H_

#include <string>
#include <iostream>
#include <filesystem>

#include <stdio.h>
#include <string.h>
#ifdef __linux__
    #include <semaphore.h> 
    #include <unistd.h> 
    #include <fcntl.h>           /* For O_* constants */
    #include <sys/stat.h>        /* For mode constants */
#endif
#include "ulti/define.h"

namespace ulti
{
    void ZeroMemory(void* data, DWORD size);
}

#endif