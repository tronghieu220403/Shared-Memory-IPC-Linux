#ifndef SHAREDMEMORYIPC_MEMORY_SHAREDMEMORY_H_
#define SHAREDMEMORYIPC_MEMORY_SHAREDMEMORY_H_

#include "ulti/framework.h"

namespace memory
{
	class SharedMemory {
	private:
        void* shared_mem_ptr_ = nullptr;
		size_t size_ = 0;
        std::string memory_name_;
	public:

		SharedMemory() = default;
		explicit SharedMemory(const std::string_view& memory_name, size_t size);
		
        void Create();

        std::vector<UCHAR> Read(void* ptr, size_t size);
        bool Write(void* ptr, void* data, size_t size);

        bool OutOfBound(void* ptr);
        bool OutOfBound(void* ptr, size_t size);

        DWORD GetSize() const;
        void* GetSharedMemoryPointer() const;
        std::string GetMemoryName() const;
    
    protected:
        void SetSize(size_t size);
        void SetSharedMemoryPointer(void* shared_mem_ptr);
        void SetMemoryName(const std::string_view& memory_name);

	};
}

#endif