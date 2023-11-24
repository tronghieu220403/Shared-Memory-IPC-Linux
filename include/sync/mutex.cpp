#include "mutex.h"

namespace sync
{
    NamedMutex::NamedMutex(const std::string_view& mutex_name):
        mutex_name_(mutex_name)
    {
        Open(mutex_name);
    }

    void NamedMutex::Open(const std::string_view& mutex_name):
        mutex_name_(mutex_name)
    {
        Close();
        if (mutex_name.size() != 0)
        {
            std::string name = "/" + mutex_name;
            p_sema_ = sem_open(name.data(), O_RDWR | O_CREAT, 0660, 1);
            if (p_sema_ == SEM_FAILED){
                p_sema_ = sem_open(name.data(), O_RDWR);
                return;
            }
        }
        else
        {
            sem_init(p_sema_, 0, 1);
        }
    }

    std::string NamedMutex::GetMutexName() const
    {
        return mutex_name_;
    }

    void NamedMutex::SetMutexName(const std::string_view &mutex_name)
    {
        mutex_name_ = mutex_name;
    }

    void NamedMutex::Lock()
    {
        sem_wait(p_sema_);
    }

    void NamedMutex::Unlock()
    {
        sem_post(p_sema_);
    }

    void NamedMutex::Close()
    {
        if (p_sema_ != nullptr)
        {
            sem_destroy(p_sema_);
            p_sema_ = nullptr;
            ulti::ZeroMemory(&sema_, sizeof(sema_));
        }
    }

    NamedMutex::~NamedMutex()
    {
        Close();
    }
}