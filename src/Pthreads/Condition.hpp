#pragma once

#include <boost/utility.hpp>

#include <pthread.h>

#include "Mutex.hpp"

namespace Pthread
{

class Condition : boost::noncopyable
{
public:
    Condition()
    {
        pthread_cond_init(&condition_, NULL);
    }

    ~Condition()
    {
        pthread_cond_destroy(&condition_);
    }

    void wait()
    {
        Mutex::Lock l(mutex_);
        pthread_cond_wait(&condition_, &mutex_.mutex_);
    }

    void signal()
    {
        Mutex::Lock l(mutex_);
        pthread_cond_signal(&condition_);
    }

    void broadcast()
    {
        Mutex::Lock l(mutex_);
        pthread_cond_broadcast(&condition_);
    }

private:
    Mutex mutex_;
    pthread_cond_t condition_;
};

} // namespace Pthread
