#include "ipcsharedmem.h"

namespace ipc
{
    IpcSharedMemory::IpcSharedMemory():
        shm_(IPC_SHM_NAME, IPC_SHM_SIZE),
        heap_manager_(shm_.GetSharedMemoryPointer(), shm_.GetSize()),
        ipc_mutex_(IPC_MUTEX_NAME)
    {

    }
		
    bool IpcSharedMemory::Send(ipc::Message& msg)
    {
        bool ret = false;
        std::vector<UCHAR> flat_msg;

        for (DWORD i = 0; i * IPC_MAX_MSG_SIZE < msg.header.total_size; i++)
        {   
            ipc::Message frag_msg = {0};
            memcpy(&frag_msg.header, &msg.header, sizeof(ipc::MessageHeader));
            frag_msg.header.frag_index = i * IPC_MAX_MSG_SIZE;
            
            frag_msg.header.frag_data_size = std::min((DWORD)IPC_MAX_MSG_SIZE, msg.header.total_size - i * IPC_MAX_MSG_SIZE);
            
            frag_msg.data.resize(frag_msg.header.frag_data_size);

            memcpy(&frag_msg.data[0], &msg.data[i * IPC_MAX_MSG_SIZE], frag_msg.header.frag_data_size);

            flat_msg = frag_msg.Flat();
            
            while(true)
            {
                ipc_mutex_.Lock();

                void* ptr = heap_manager_.Alloc(flat_msg.size());
                if (ptr == nullptr)
                {
                    ipc_mutex_.Unlock();
                    continue;
                }

                if (shm_.Write(ptr, &flat_msg[0], flat_msg.size()) == true)
                {
                    ipc_mutex_.Unlock();
                    break;
                }
                else
                {
                    ipc_mutex_.Unlock();
                    continue;
                }
            }
            
        }
        return true;
    }

    bool IpcSharedMemory::Send(DWORD receiver_pid, const std::vector<UCHAR>& data)
    {
        ipc::Message msg = {0};
        
        msg.header.recv_pid = receiver_pid;
        msg.header.send_pid = ::getpid();
        msg.header.total_size = data.size();

        msg.data.resize(data.size());
        memcpy(&msg.data[0], data.data(), data.size());

        msg.header.total_checksum = msg.ipc::Message::CalculateChecksum();
        
        return IpcSharedMemory::Send(msg);
    }

    std::vector<ipc::Message> IpcSharedMemory::Receive()
    {
        std::vector<ipc::Message> recv_packet;
        ipc::Message msg;

        heap::HeapHeader* heap_header_ptr = (heap::HeapHeader*)heap_manager_.GetStartPointer();
        std::vector<UCHAR> heap_header_data = shm_.Read(heap_header_ptr, sizeof(heap::HeapHeader));

        ipc::MessageHeader* msg_header_ptr;
        std::vector<UCHAR> msg_header_data;

        void* data_ptr;
        std::vector<UCHAR> data;

        std::vector<void *> free_queue;

        ipc_mutex_.Lock();
        // ulti::PrintDebug("Enter IPC lock");

        while(true)
        {
            // Check if the chunk is in use or not
            if ((((heap::HeapHeader*)&heap_header_data[0])->flag & IN_USE)==0)
            {
                goto NEXT_LABEL;
            }

            // Read the content of MessageHeader
            msg_header_ptr = (MessageHeader *)((size_t)heap_header_ptr+sizeof(heap::HeapHeader));
            msg_header_data = shm_.Read(msg_header_ptr, sizeof(MessageHeader));

            // Check if the recv_pid is the pid of the receiver, if not, check if that process is still alive and go to next chunk
            if (::getpid() != ((MessageHeader *)&msg_header_data[0])->recv_pid)
            {
                // Check if the process is still active
                if (process::Process(((MessageHeader *)&msg_header_data[0])->recv_pid).IsActive() == false)
                {
                    free_queue.push_back(msg_header_ptr);
                }
                goto NEXT_LABEL;
            }

            // Read the content of message
            data_ptr = (void *)((size_t)msg_header_ptr + sizeof(ipc::MessageHeader));
            data = shm_.Read(data_ptr , msg_header_ptr->frag_data_size);

            msg = {0};
            memcpy(&msg.header, ((MessageHeader *)&msg_header_data[0]), sizeof(ipc::MessageHeader));
            msg.data = data;

            recv_packet.push_back(msg);

            // Recveived, push to delete queue
            free_queue.push_back(msg_header_ptr);
            
            NEXT_LABEL:

            if (((heap::HeapHeader*)&heap_header_data[0])->next_distance == 0)
            {
                break;
            }

            // Get the pointer of next heap header
            heap_header_ptr = (heap::HeapHeader*)
                                ((size_t)heap_header_ptr + 
                                    ((heap::HeapHeader*)&heap_header_data[0])->next_distance);

            // Read the content of next heap header
            heap_header_data = shm_.Read(heap_header_ptr, sizeof(heap::HeapHeader));
        }

        for (auto value: free_queue)
        {
            heap_manager_.Free(value);
        }

        ipc_mutex_.Unlock();
        // ulti::PrintDebug("Exit IPC lock");

        return IpcSharedMemory::ResolveFragmentMessages(recv_packet);
    }

    std::vector<ipc::Message> IpcSharedMemory::ResolveFragmentMessages(std::vector<ipc::Message> extra_packet)
    {
        for (auto& packet: extra_packet)
        {
            packet_list_.push_back(packet);
        }
        return IpcSharedMemory::ResolveFragmentMessages();
    }

    std::vector<ipc::Message> IpcSharedMemory::ResolveFragmentMessages()
    {
        for (auto& packet: packet_list_)
        {
            bool merged = false;
            for (int i = 0; i < packet_merge_list_.size(); i++)
            {
                if (packet_merge_list_[i][0].header.total_checksum == packet.header.total_checksum &&
                    packet_merge_list_[i][0].header.send_pid == packet.header.send_pid)
                {
                    packet_merge_list_[i].push_back(packet);
                    merged = true;
                }
            }
            if (merged == false)
            {
                std::vector<ipc::Message> packet_merge_vector;
                packet_merge_vector.push_back(packet);
                packet_merge_list_.push_back(packet_merge_vector);
            }
        }
        packet_list_.clear();

        return IpcSharedMemory::MergeFragmentMessages();
    }

    std::vector<ipc::Message> IpcSharedMemory::MergeFragmentMessages()
    {
        std::vector<ipc::Message> msg_list;
        std::vector<int> delete_index_vector;
        for (int i = 0; i < packet_merge_list_.size(); i++) 
        {
            DWORD total_size_recv = 0;
            DWORD real_total_size = packet_merge_list_[0][0].header.total_size;

            for (auto& packet: packet_merge_list_[i])
            {
                total_size_recv += packet.header.frag_data_size;
                if (real_total_size != packet.header.total_size)
                {
                    delete_index_vector.push_back(i);
                    total_size_recv = -1;
                    real_total_size = 0;
                    break;
                }
            }

            // If full message is received
            if (total_size_recv == real_total_size)
            {
                ipc::Message full_msg = {0};
                
                memcpy(&full_msg.header, &packet_merge_list_[i][0].header, sizeof(ipc::MessageHeader));
                full_msg.header.total_checksum = 0;
                full_msg.header.frag_data_size = real_total_size;
                full_msg.header.total_size = real_total_size;
                full_msg.header.frag_index = 0;

                full_msg.data.resize(real_total_size);
                for (auto& packet: packet_merge_list_[i])
                {
                    memcpy(&full_msg.data[packet.header.frag_index], &packet.data[0], packet.header.frag_data_size);
                }

                msg_list.push_back(full_msg);

                delete_index_vector.push_back(i);
            }
            // If message is not fully received, check if the sender is active.
            // If the sender is inactive, delete the whole packet_merge_list.
            else if (process::Process(packet_merge_list_[0][0].header.send_pid).IsActive() == false)
            {
                delete_index_vector.push_back(i);
            }
        }

        for (int i = delete_index_vector.size() - 1; i >= 0; i--)
        {
            packet_merge_list_.erase(packet_merge_list_.begin() + delete_index_vector[i]);
        }

        return msg_list;
    }

    heap::HeapManager IpcSharedMemory::GetHeapManager() const
    {
        return heap_manager_;
    }

    void IpcSharedMemory::SetHeapManager(const heap::HeapManager& heap_manager)
    {
        heap_manager_ = heap_manager;
    }

    synchronization::NamedMutex IpcSharedMemory::GetIpcMutex() const
    {
        return ipc_mutex_;
    }

    void IpcSharedMemory::SetIpcMutex(const synchronization::NamedMutex& ipc_mutex)
    {
        ipc_mutex_ = ipc_mutex;
    }

    memory::SharedMemory IpcSharedMemory::GetSharedMemory() const
    {
        return shm_;
    }

    void IpcSharedMemory::SetSharedMemory(const memory::SharedMemory& shm)
    {
        shm_ = shm;
    }

}

