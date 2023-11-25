#include "sharedmemory.h"

namespace memory
{
    SharedMemory::SharedMemory(const std::string_view& memory_name, size_t size):
        memory_name_(memory_name),
        size_(size)
    {

    }

    void SharedMemory::Create()
    {
        int d;
        std::string name = std::string("\\") + memory_name_;

        d = shm_open(&name[0], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

        if (d == -1)
        {
            return;
        }

        shared_mem_ptr_ = mmap(NULL, size_, PROT_READ | PROT_WRITE, MAP_SHARED, d, 0);

    }

    std::vector<UCHAR> SharedMemory::Read(DWORD offset, size_t size)
    {
        if (OutOfBound(offset, size))
        {
            return std::vector<UCHAR>();
        }
        std::vector<UCHAR> data(size);
        if (memcpy(&data[0], (void *)((size_t)shared_mem_ptr_ + offset), size) == nullptr)
        {
            return std::vector<UCHAR>();
        }
        return data;
    }

    bool SharedMemory::Write(DWORD offset, void* src, size_t size)
    {
        if (OutOfBound(offset, size))
        {
            return false;
        }
        if (memcpy((void *)((size_t)shared_mem_ptr_ + offset), src, size) == nullptr)
        {
            return false;
        }
        return true;
    }

    bool SharedMemory::OutOfBound(DWORD offset)
    {
        if (offset > size_)
        {
            return true;
        }
        return false;
    }

    bool SharedMemory::OutOfBound(DWORD offset, size_t size)
    {
        return SharedMemory::OutOfBound(offset) && SharedMemory::OutOfBound((offset + size));
    }

    void SharedMemory::SetSize(size_t size)
    {
        size_ = size;
    }

    DWORD SharedMemory::GetSize() const
    {
        return size_;
    }

    void SharedMemory::SetSharedMemoryPointer(void* shared_mem_ptr)
    {
        shared_mem_ptr_ = shared_mem_ptr;
    }

    void* SharedMemory::GetSharedMemoryPointer() const
    {
        return shared_mem_ptr_;
    }

    void SharedMemory::SetMemoryName(const std::string_view& memory_name)
    {
        memory_name_ = memory_name;
    }

    std::string SharedMemory::GetMemoryName() const
    {
        return memory_name_;
    }

}
