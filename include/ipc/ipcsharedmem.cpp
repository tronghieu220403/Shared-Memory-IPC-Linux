#include "ipcsharedmem.h"

namespace ipc
{
    IpcSharedMemory::IpcSharedMemory():
        shm_(IPC_SHM_NAME, IPC_SHM_SIZE),
        heap_manager_(shm_.GetSharedMemoryPointer(), shm_.GetSize()),
        ipc_mutex_("ipc_sync_mutex")
    {

    }
		
    bool IpcSharedMemory::Send(const MessageStructure &msg)
    {
        ipc_mutex_.Lock();
        std::vector<UCHAR> flat_msg = msg.Flat();
        void* ptr = heap_manager_.Alloc(flat_msg.size());
        if (ptr == nullptr)
        {
            ipc_mutex_.Unlock();
            return false;
        }

        if (shm_.Write(ptr, &flat_msg[0], flat_msg.size()) == true)
        {
            ipc_mutex_.Unlock();
            return true;
        }

        ipc_mutex_.Unlock();
        return false;
    }

    bool IpcSharedMemory::Send(DWORD receiver_pid, const std::vector<UCHAR>& data)
    {
        MessageStructure msg = {0};
        msg.header.recv_pid = receiver_pid;
        msg.header.recv_pid = ::getpid();
        msg.data = data;
        return IpcSharedMemory::Send(msg);
    }

    std::vector<ipc::MessageStructure> IpcSharedMemory::Receive()
    {
        std::vector<ipc::MessageStructure> ans;
        ipc::MessageStructure msg;

        heap::HeapHeader* heap_header_ptr = (heap::HeapHeader*)heap_manager_.GetStartPointer();
        std::vector<UCHAR> heap_header_data = shm_.Read(heap_header_ptr, sizeof(heap::HeapHeader*));

        ipc::MessageHeader* msg_header_ptr;
        std::vector<UCHAR> msg_header_data;

        void* data_ptr;
        std::vector<UCHAR> data;

        std::vector<void *> free_queue;

        ipc_mutex_.Lock();

        while(true)
        {
            // The heap must be in use
            if ((((heap::HeapHeader*)&heap_header_data[0])->flag & IN_USE)==0)
            {
                goto NEXT_LABEL;
            }

            // Read the content of MessageHeader
            msg_header_ptr = (MessageHeader *)((size_t)heap_header_ptr+sizeof(heap::HeapHeader));
            msg_header_data = shm_.Read(msg_header_ptr, sizeof(MessageHeader));

            // Check if the recv_pid is the pid of the receiver
            if (::getpid() != ((MessageHeader *)&msg_header_data[0])->recv_pid)
            {
                goto NEXT_LABEL;
            }

            // Read the content of message
            data_ptr = (void *)((size_t)msg_header_ptr + sizeof(ipc::MessageHeader));
            data = shm_.Read(data_ptr , msg_header_ptr->data_size);

            msg = {0};
            msg.header.data_size = ((MessageHeader *)&msg_header_data[0])->data_size;
            msg.header.recv_pid = ((MessageHeader *)&msg_header_data[0])->recv_pid;
            msg.header.send_pid = ((MessageHeader *)&msg_header_data[0])->send_pid;
            msg.data = data;

            ans.push_back(msg);

            // Recveived, push to delete queue
            free_queue.push_back(msg_header_ptr);
            
            NEXT_LABEL:
            
            if (((heap::HeapHeader*)&heap_header_data[0])->next_distance == 0)
            {
                break;
            }

            // Get the pointer of the next heap header
            heap_header_ptr = (heap::HeapHeader*)
                                ((size_t)heap_header_ptr + 
                                    ((heap::HeapHeader*)&heap_header_data[0])->next_distance);

            // Read the content of the next heap header
            heap_header_data = shm_.Read(heap_header_ptr, sizeof(heap::HeapHeader*));
        }

        for (auto value: free_queue)
        {
            heap_manager_.Free(value);
        }

        ipc_mutex_.Unlock();
        return ans;
    }

    std::vector<UCHAR> MessageStructure::Flat() const
    {
        std::vector<UCHAR> flat_data(sizeof(DWORD)*3 + data.size());
        memcpy(&flat_data[0], &header, sizeof(MessageStructure));
        memcpy(&flat_data[12], &data[0], data.size());
    }

}
