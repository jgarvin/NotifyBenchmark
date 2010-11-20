#pragma once

#include <boost/utility.hpp>

#include <pthread.h>

namespace Pthread {

class Mutex : boost::noncopyable
{
public:
    Mutex()
    {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&mutex_);
    }

    class Lock : boost::noncopyable
    {
    public:
        Lock(Mutex& mutex)
            : mutex_(mutex.mutex_)
        {
            pthread_mutex_lock(&mutex_);
        }

        ~Lock()
        {
            pthread_mutex_unlock(&mutex_);
        }
    private:
        pthread_mutex_t& mutex_;
    };

private:
    pthread_mutex_t mutex_;
    friend class Condition;
};

} // namespace Pthread
