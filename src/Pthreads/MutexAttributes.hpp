#pragma once

#include <pthread.h>

namespace Pthread
{

template<class Callback>
class Thread;

class MutexAttributes
{
public:
    MutexAttributes()
    {
        pthread_mutexattr_init(&attrs_);
    }

    ~MutexAttributes()
    {
        pthread_mutexattr_destroy(&attrs_);
    }

private:
    pthread_mutexattr_t attrs_;

    template<class Callback>
    friend class Pthread::Thread;
};

} // namespace Pthread
