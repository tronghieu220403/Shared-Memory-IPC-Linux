
#ifndef SHAREDMEMORYIPC_SYNC_MUTEX_H_
#define SHAREDMEMORYIPC_SYNC_MUTEX_H_

#include "ulti/framework.h"

namespace sync
{
    class NamedMutex
    {
    private:
        std::string mutex_name_;

        sem_t *p_sema_;
        
    public:
        NamedMutex() = default;
        explicit NamedMutex(const std::string_view& mutex_name);
        
        void Open(const std::string_view& mutex_name);
        void Close();

        void Lock();
        void Unlock();

        std::string GetMutexName() const;
        void SetMutexName(const std::string_view& mutex_name);

        ~NamedMutex();
    };

}

#endif