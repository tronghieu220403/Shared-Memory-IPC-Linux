#ifndef SHAREDMEMORYIPC_IPC_IPCSHAREDMEMORY_H_
#define SHAREDMEMORYIPC_IPC_IPCSHAREDMEMORY_H_

#include "ulti/framework.h"
#include "ipc/define.h"
#include "heap/heap.h"
#include "memory/sharedmemory.h"
#include "sync/mutex.h"
#include "process/process.h"

namespace ipc
{

	class IpcSharedMemory {
	private:

        memory::SharedMemory shm_;
        heap::HeapManager heap_manager_;
        synchronization::NamedMutex ipc_mutex_;

	public:

		IpcSharedMemory();
		
        bool Send(const MessageStructure& msg);
        bool Send(DWORD receiver_pid, const std::vector<UCHAR>& data);
        std::vector<ipc::MessageStructure> Receive();

    // protected:

        heap::HeapManager GetHeapManager() const;
        void SetHeapManager(const heap::HeapManager& heap_manager);

        synchronization::NamedMutex GetIpcMutex() const;
        void SetIpcMutex(const synchronization::NamedMutex& ipc_mutex);

        memory::SharedMemory GetSharedMemory() const;
        void SetSharedMemory(const memory::SharedMemory& shm);

	};
}

#endif