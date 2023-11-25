#include "mutex.h"

namespace synchronization
{
    NamedMutex::NamedMutex(const std::string_view& mutex_name):
        mutex_name_(mutex_name)
    {
        Open();
    }

    void NamedMutex::Open()
    {
    #ifdef __linux__
        Close();
        if (mutex_name_.size() != 0)
        {
            std::string name = std::string("/") + std::string(mutex_name_);
            p_sema_ = ::sem_open(name.data(), O_RDWR | O_CREAT, 0660, 1);
            if (p_sema_ == SEM_FAILED){
                p_sema_ = ::sem_open(name.data(), O_RDWR);
                return;
            }
        }
        else
        {
            ::sem_init(p_sema_, 0, 1);
        }
    #endif
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
    #ifdef __linux__
        ::sem_wait(p_sema_);
    #endif
    }

    void NamedMutex::Unlock()
    {
    #ifdef __linux__
        ::sem_post(p_sema_);
    #endif
    }

    void NamedMutex::Close()
    {
    #ifdef __linux__
        if (p_sema_ != nullptr)
        {
            ::sem_close(p_sema_);
            p_sema_ = nullptr;
        }
    #endif
    }

    NamedMutex::~NamedMutex()
    {
        // Close();
    }
}