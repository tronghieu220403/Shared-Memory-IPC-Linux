#ifndef SHAREDMEMORYIPC_IPC_DEFINE_H_
#define SHAREDMEMORYIPC_IPC_DEFINE_H_

#include "ulti/framework.h"

namespace ipc
{
        
    #define IPC_SHM_SIZE 0x10000
    #define IPC_SHM_NAME "hieu_ipc_shm"
    #define IPC_MUTEX_NAME "IPC_MUTEX_NAME"

    struct MessageHeader
    {
        DWORD recv_pid;
        DWORD send_pid;
        DWORD data_size;
    };

    struct MessageStructure
    {
        MessageHeader header;
        std::vector<UCHAR> data;
        
        std::vector<UCHAR> Flat() const;
    };

}

#endif