#ifndef SHAREDMEMORYIPC_IPC_IPCSHAREDMEMORY_H_
#define SHAREDMEMORYIPC_IPC_IPCSHAREDMEMORY_H_

#include "ulti/framework.h"
#include "heap/heap.h"
#include "memory/sharedmemory.h"
#include "sync/mutex.h"

namespace ipc
{
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
    
    #define IPC_SHM_SIZE 0x10000
    #define IPC_SHM_NAME "hieu_ipc_shm"

	class IpcSharedMemory {
	private:

        heap::HeapManager heap_manager_;
        memory::SharedMemory shm_;
        synchronization::NamedMutex ipc_mutex_;

	public:

		IpcSharedMemory();
		
        bool Send(const MessageStructure& msg);
        bool Send(DWORD receiver_pid, const std::vector<UCHAR>& data);
        std::vector<ipc::MessageStructure> Receive();

    protected:

        heap::HeapManager GetHeapManager() const;
        void SetHeapManager(const heap::HeapManager& heap_manager);

        synchronization::NamedMutex GetIpcMutex() const;
        void SetIpcMutex(const synchronization::NamedMutex& ipc_mutex);

        memory::SharedMemory GetSharedMemory() const;
        void SetSharedMemory(const memory::SharedMemory& shm);

	};
}

#endif