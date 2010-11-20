#pragma once

#include <boost/utility.hpp>
#include <boost/call_traits.hpp>

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
    Thread(MutexAttributes const& attrs = MutexAttributes(),
            typename boost::call_traits
            <typename Callback::Data>::param_type const& data
            = typename Callback::Data())
        : data_(data)
    {
        pthread_create(&thread_, &attrs.attrs_, &dispatch<Callback>, &data_);
    }

    ~Thread()
    {
        pthread_join(thread_, NULL);
    }

private:
    typename Callback::Data data_;
    pthread_t thread_;
};

} // namespace Pthread
