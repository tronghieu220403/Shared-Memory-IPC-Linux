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
        int fd;
        std::string name = std::string("\\") + memory_name_;

        // Open or create a shared memory object
        fd = ::shm_open(&name[0], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            return;
        }
        
        // Create a shared memory mapping
        shared_mem_ptr_ = ::mmap(NULL, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        // Set size for the shared memory area
        ::ftruncate(fd, size_);
    }

    std::vector<UCHAR> SharedMemory::Read(void* ptr, size_t size)
    {
        if (OutOfBound(ptr, size))
        {
            return std::vector<UCHAR>();
        }
        std::vector<UCHAR> data(size);
        if (::memcpy(&data[0], ptr, size) == nullptr)
        {
            return std::vector<UCHAR>();
        }
        return data;
    }

    bool SharedMemory::Write(void* ptr, void* src, size_t size)
    {
        if (OutOfBound(ptr, size))
        {
            return false;
        }
        if (::memcpy(ptr, src, size) == nullptr)
        {
            return false;
        }
        return true;
    }

    bool SharedMemory::OutOfBound(void* ptr)
    {
        if ((size_t)ptr < (size_t)shared_mem_ptr_ ||
            (size_t)ptr > ((size_t)shared_mem_ptr_ + size_))
        {
            return true;
        }
        return false;
    }

    bool SharedMemory::OutOfBound(void* ptr, size_t size)
    {
        return SharedMemory::OutOfBound(ptr) && SharedMemory::OutOfBound((void *)((size_t)ptr + size));
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
