#include "heap.h"

namespace heap
{
    HeapManager::HeapManager(void *ptr, DWORD size):
        ptr_(ptr),
        size_(size),
        heap_mutex_("heap_sync")
    {
        heap_mutex_.Open();

        DWORD true_size = (size - sizeof(heap::HeapHeader))/0x10*0x10;

        heap::HeapHeader* first_ptr = (heap::HeapHeader*)ptr_;
        heap::HeapHeader* last_ptr = (heap::HeapHeader*)((size_t)ptr_ + true_size);
        ulti::ZeroMemory(first_ptr, sizeof(heap::HeapHeader));
        ulti::ZeroMemory(last_ptr, sizeof(heap::HeapHeader));

        first_ptr->flag = 0;
        first_ptr->prev_distance = 0;
        first_ptr->next_distance = static_cast<DWORD>((size_t)last_ptr - (size_t)first_ptr);

        last_ptr->flag = IN_USE;
        last_ptr->prev_distance = first_ptr->next_distance;
        last_ptr->next_distance = 0;

    }

    void* HeapManager::Alloc(DWORD size)
    {
        if (size == 0)
        {
            return nullptr;
        }

        heap_mutex_.Lock();
        heap::HeapHeader* this_ptr = (heap::HeapHeader *)ptr_;
        DWORD true_size = (size+sizeof(heap::HeapHeader))%0x10 == 0 ? (size+sizeof(heap::HeapHeader)) : ((size+sizeof(heap::HeapHeader))/0x10+1)*0x10;

        while(true)
        {
            if (OutOfBound((this_ptr)))
            {
                break;
            }

            if ( (this_ptr->flag & IN_USE) == 0 &&
                this_ptr->next_distance >= true_size)
            {
                if (this_ptr->next_distance >= true_size + sizeof(heap::HeapHeader))
                {
                    // split the heap into 2 part

                    heap::HeapHeader* second_half_ptr = (heap::HeapHeader*)((size_t)this_ptr + true_size);
                    ulti::ZeroMemory(second_half_ptr, sizeof(heap::HeapHeader));
                    second_half_ptr->prev_distance = true_size;
                    second_half_ptr->next_distance = this_ptr->next_distance - true_size;

                    heap::HeapHeader* first_half_ptr = this_ptr;
                    first_half_ptr->reserve[0] = 0;
                    first_half_ptr->flag = IN_USE;
                    first_half_ptr->next_distance = true_size;
                    first_half_ptr->prev_distance = this_ptr->prev_distance;

                    return (void*)((size_t)first_half_ptr + sizeof(heap::HeapHeader));
                }
                else
                {
                    this_ptr->flag = IN_USE;
                    return this_ptr;
                }
            }

            if (this_ptr->next_distance == 0)
            {
                return nullptr;
            }
            this_ptr = (heap::HeapHeader *)((size_t)this_ptr + this_ptr->next_distance);
        }
        heap_mutex_.Unlock();
        return nullptr;
    }

    void HeapManager::Free(void *data_ptr)
    {
        heap_mutex_.Lock();

        heap::HeapHeader* header_ptr;

        header_ptr = (heap::HeapHeader *)((size_t)data_ptr-sizeof(HeapHeader));

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
            heap::HeapHeader* next_ptr = (heap::HeapHeader *)((size_t)header_ptr + header_ptr->next_distance);
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

        header_ptr = (heap::HeapHeader*)((size_t)data_ptr - sizeof(HeapHeader));

        while(true)
        {
            if (header_ptr->prev_distance == 0 || OutOfBound(header_ptr))
            {
                break;
            }
            heap::HeapHeader* prev_ptr = (heap::HeapHeader *)((size_t)header_ptr - header_ptr->prev_distance);

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

        heap_mutex_.Unlock();
    }

    bool HeapManager::OutOfBound(void* header_ptr)
    {
        if (size_ <= sizeof(heap::HeapHeader) || (size_t)header_ptr < (size_t)ptr_ || size_t(header_ptr) > (size_t)(ptr_) + size_ - sizeof(heap::HeapHeader))
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

