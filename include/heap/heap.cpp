#include "heap.h"

namespace heap
{
    void HeapManager::Init()
    {
        size_ = (size_ - sizeof(heap::ChunkHeader))/0x10*0x10 - 0x10;

        // Check if the heap has been build
        DWORD check1 = *(DWORD *)ptr_;
        DWORD check2 = *(DWORD *)((size_t)ptr_ + sizeof(DWORD));
        if (check1 == 0x22042003 && check2 == 0x20030422)
        {
            ptr_ = (void *)((size_t)ptr_ + sizeof(DWORD) * 2);
            return;
        }
        *(DWORD *)ptr_ = 0x22042003;
        *(DWORD *)((size_t)ptr_ + sizeof(DWORD)) = 0x20030422;
        ptr_ = (void *)((size_t)ptr_ + sizeof(DWORD) * 2);

        heap::ChunkHeader* first_ptr = (heap::ChunkHeader*)ptr_;
        heap::ChunkHeader* last_ptr = (heap::ChunkHeader*)((size_t)ptr_ + size_ - sizeof(heap::ChunkHeader));
        ulti::ZeroMemory(first_ptr, sizeof(heap::ChunkHeader));
        ulti::ZeroMemory(last_ptr, sizeof(heap::ChunkHeader));

        first_ptr->flag = 0;
        first_ptr->prev_distance = 0;
        first_ptr->next_distance = static_cast<DWORD>((size_t)last_ptr - (size_t)first_ptr);

        last_ptr->flag = IN_USE;
        last_ptr->prev_distance = first_ptr->next_distance;
        last_ptr->next_distance = 0;
    }

    HeapManager::HeapManager(void *ptr, DWORD size):
        ptr_(ptr),
        size_(size)
    {
        HeapManager::Init();
    }

    void* HeapManager::Alloc(DWORD size)
    {
        if (size == 0)
        {
            return nullptr;
        }

        heap::ChunkHeader* this_ptr = (heap::ChunkHeader *)ptr_;
        DWORD true_size = (size+sizeof(heap::ChunkHeader))%0x10 == 0 ? (size+sizeof(heap::ChunkHeader)) : ((size+sizeof(heap::ChunkHeader))/0x10+1)*0x10;

        while(true)
        {
            if (OutOfBound((this_ptr)))
            {
                break;
            }

            if ( (this_ptr->flag & IN_USE) == 0 &&
                this_ptr->next_distance >= true_size)
            {
                // if enough room for split in to 2 part
                if (this_ptr->next_distance >= true_size + sizeof(heap::ChunkHeader))
                {
                    // split the heap into 2 parts

                    heap::ChunkHeader* second_half_ptr = (heap::ChunkHeader*)((size_t)this_ptr + true_size);
                    ulti::ZeroMemory(second_half_ptr, sizeof(heap::ChunkHeader));
                    second_half_ptr->prev_distance = true_size;
                    second_half_ptr->next_distance = this_ptr->next_distance - true_size;

                    heap::ChunkHeader* first_half_ptr = this_ptr;
                    first_half_ptr->reserve[0] = 0;
                    first_half_ptr->flag = IN_USE;
                    first_half_ptr->next_distance = true_size;
                    first_half_ptr->prev_distance = this_ptr->prev_distance;

                    return (void*)((size_t)first_half_ptr + sizeof(heap::ChunkHeader));
                }
                else // else if not enough room for split in to 2 parts, we will only change the flag of the pointer
                {
                    this_ptr->flag = IN_USE;
                    return (void*)((size_t)this_ptr + sizeof(heap::ChunkHeader));
                }
            }

            if (this_ptr->next_distance == 0)
            {
                return nullptr;
            }
            this_ptr = (heap::ChunkHeader *)((size_t)this_ptr + this_ptr->next_distance);
        }
        return nullptr;
    }

    void HeapManager::Free(void *data_ptr)
    {
        heap::ChunkHeader* header_ptr;

        header_ptr = (heap::ChunkHeader *)((size_t)data_ptr-sizeof(ChunkHeader));

        if ( (header_ptr->flag & IN_USE) == 1)
        {
            header_ptr->flag = header_ptr->flag ^ IN_USE;
        }

        while(true)
        {
            if (header_ptr->next_distance == 0 || OutOfBound(header_ptr))
            {
                break;
            }
            heap::ChunkHeader* next_ptr = (heap::ChunkHeader *)((size_t)header_ptr + header_ptr->next_distance);
            if ( (next_ptr->flag & IN_USE) == 0 )
            {
                next_ptr->prev_distance += header_ptr->prev_distance;
            }
            else 
            {
                break;
            }
            header_ptr = next_ptr;
        }

        header_ptr = (heap::ChunkHeader*)((size_t)data_ptr - sizeof(ChunkHeader));

        while(true)
        {
            if (header_ptr->prev_distance == 0 || OutOfBound(header_ptr))
            {
                break;
            }
            heap::ChunkHeader* prev_ptr = (heap::ChunkHeader *)((size_t)header_ptr - header_ptr->prev_distance);

            if ( (prev_ptr->flag & IN_USE) == 0 )
            {
                prev_ptr->next_distance += header_ptr->next_distance;
            }
            else 
            {
                break;
            }
            header_ptr = prev_ptr;
        }
    }

    bool HeapManager::OutOfBound(void* header_ptr)
    {
        if (size_ <= sizeof(heap::ChunkHeader) || (size_t)header_ptr < (size_t)ptr_ || size_t(header_ptr) > (size_t)(ptr_) + size_ - sizeof(heap::ChunkHeader))
        {
            return true;
        }
        return false;
    }


    void *HeapManager::GetStartPointer() const
    {
        return ptr_;
    }

    void HeapManager::SetStartPointer(void* ptr)
    {
        ptr_ = ptr;
    }

    DWORD HeapManager::GetTotalHeapSize() const
    {
        return size_;
    }
    
    void HeapManager::SetTotalHeapSize(DWORD size)
    {
        size_ = size;
    }


}

