
#ifndef SHAREDMEMORYIPC_SYNC_MUTEX_H_
#define SHAREDMEMORYIPC_SYNC_MUTEX_H_

#include "ulti/framework.h"

namespace synchronization
{
    class NamedMutex
    {
    private:
        std::string mutex_name_;

        #ifdef __linux__
            sem_t *p_sema_;
        #endif            
    public:
        NamedMutex() = default;
        explicit NamedMutex(const std::string_view& mutex_name);
        
        void Open();
        void Close();

        void Lock();
        void Unlock();

        std::string GetMutexName() const;
        void SetMutexName(const std::string_view& mutex_name);

        ~NamedMutex();
    };

}

#endif