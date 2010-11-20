#include <utility>

#include <boost/shared_ptr.hpp>

#include "Pthreads/Thread.hpp"
#include "Pthreads/Mutex.hpp"

struct NoOp
{
    typedef int Data;
    static void action(int) {}
};

#include <iostream>

template<class RealCallback>
struct WaitForMutex
{
    typedef std::pair<Pthread::Mutex&, typename RealCallback::Data> Data;
    static void action(Data& startup_pair)
    {
        { Pthread::Mutex::Lock l(startup_pair.first); }

        RealCallback::action(startup_pair.second);
    }
};

// FIXME: Later can lift things out of the class, define there for now.
class PipeTest : boost::noncopyable
{
public:
    PipeTest()
    {
        Pthread::Mutex starting_gate;

        typedef WaitForMutex<NoOp> DelayedNoOp;

        boost::shared_ptr<Pthread::Mutex::Lock> l(new Pthread::Mutex::Lock(starting_gate));

        Pthread::Thread< DelayedNoOp > threadA(DelayedNoOp::Data(starting_gate, 0));
        Pthread::Thread< DelayedNoOp > threadB(DelayedNoOp::Data(starting_gate, 0));

        l.reset();
    }

    void activate()
    {

    }

private:
};

int main()
{
    // Create two threads to signal between.

    PipeTest test;

    return 0;
}
