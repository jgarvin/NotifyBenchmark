#pragma once

#include <boost/utility.hpp>
#include <boost/optional.hpp>

#include <pthread.h>

#include "MutexAttributes.hpp"

namespace Pthread {

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
class Thread : boost::noncopyable
{
public:
    Thread(boost::optional<typename Callback::Data> data
               = boost::none)
    {
        // FIXME: Need ThreadAttributes
        typename Callback::Data* data_pointer = data ? &*data : NULL;

        pthread_create(&thread_, NULL, &dispatch<Callback>, data_pointer);
    }

    ~Thread()
    {
        pthread_join(thread_, NULL);
    }

private:
    pthread_t thread_;
};

} // namespace Pthread
