#ifndef SHAREDMEMORYIPC_IPC_DEFINE_H_
#define SHAREDMEMORYIPC_IPC_DEFINE_H_

#include "ulti/framework.h"
#include <ctime>

namespace ipc
{
        
    #define IPC_SHM_SIZE 0x10000
    #define IPC_SHM_NAME "hieu_ipc_shmggg"
    #define IPC_MUTEX_NAME "hieu_ipc_mutex"

    #define IPC_MAX_MSG_SIZE 0x1000

    struct MessageHeader
    {
        DWORD recv_pid;
        DWORD send_pid;
        DWORD data_size;
        DWORD total_size;
        DWORD index;
        DWORD checksum;
    };

    struct Message
    {
        MessageHeader header;
        std::vector<UCHAR> data;
        std::vector<UCHAR> Flat() const;
        DWORD CalculateChecksum();
    };

}

#endif