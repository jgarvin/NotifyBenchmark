#include <boost/call_traits.hpp>

#include <pthread.h>

class PthreadMutexAttributes
{
public:
    PthreadMutexAttributes()
    {
        pthread_mutexattr_init(&attrs_);
    }

    ~PthreadMutexAttributes()
    {
        pthread_mutexattr_destroy(&attrs_);
    }

private:
    pthread_mutexattr_t attrs_;
};

namespace {

template<class Callback>
static void* dispatch(void* data)
{
    typename Callback::Data* typed_data
        = static_cast<typename Callback::Data*>(data);

    Callback::action(*typed_data);
}

}

template<class Callback>
class Pthread
{
public:
    Pthread(PthreadMutexAttributes const& attrs = PthreadMutexAttributes(),
            typename boost::call_traits
                <typename Callback::Data>::param_type const& data
                    = typename Callback::Data())
        : data_(data)
    {
        pthread_create(&thread_, NULL, &dispatch<Callback>, &data_);
    }

    ~Pthread()
    {
        pthread_join(thread_, NULL);
    }

private:
    typename Callback::Data data_;
    pthread_t thread_;
};

struct NoOp
{
    typedef int Data;
    static void action(int) {}
};

// FIXME: Later can lift things out of the class, define there for now.
class PipeTest
{
public:
    PipeTest()
    {
        PthreadMutexAttributes attrs;

        Pthread<NoOp> threadA(attrs);
        Pthread<NoOp> threadB(attrs);
    }

    void activate()
    {

    }

private:
    pthread_t threadA_;
    pthread_t threadB_;
    pthread_mutex_t start_mutex_;
};

int main()
{
    // Create two threads to signal between.

    return 0;
}
