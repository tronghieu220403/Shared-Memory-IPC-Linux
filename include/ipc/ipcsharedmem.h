#ifndef SHAREDMEMORYIPC_IPC_IPCSHAREDMEMORY_H_
#define SHAREDMEMORYIPC_IPC_IPCSHAREDMEMORY_H_

#include "ulti/framework.h"
#include "ipc/define.h"
#include "heap/heap.h"
#include "memory/sharedmemory.h"
#include "sync/mutex.h"
#include "process/process.h"
#include <algorithm>


namespace ipc
{

	class IpcSharedMemory 
    {
	private:
        std::vector<ipc::Message> recv_msg_list_;
        std::vector< std::vector<ipc::Message> > packet_merge_list_;
        memory::SharedMemory shm_;
        heap::HeapManager heap_manager_;
        synchronization::NamedMutex ipc_mutex_;

	public:

		IpcSharedMemory();
		
        bool Send(ipc::Message& msg);
        bool Send(DWORD receiver_pid, const std::vector<UCHAR>& data);
        std::vector<ipc::Message> Receive();

    protected:
        std::vector<ipc::Message> ResolveFragmentMessages();
        std::vector<ipc::Message> MergeFragmentMessages();

        heap::HeapManager GetHeapManager() const;
        void SetHeapManager(const heap::HeapManager& heap_manager);

        synchronization::NamedMutex GetIpcMutex() const;
        void SetIpcMutex(const synchronization::NamedMutex& ipc_mutex);

        memory::SharedMemory GetSharedMemory() const;
        void SetSharedMemory(const memory::SharedMemory& shm);

	};
}

#endif