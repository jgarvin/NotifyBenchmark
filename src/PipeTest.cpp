#include "Pthreads/Thread.hpp"

struct NoOp
{
    typedef int Data;
    static void action(int) {}
};

// FIXME: Later can lift things out of the class, define there for now.
class PipeTest : boost::noncopyable
{
public:
    PipeTest()
    {
        Pthread::Thread<NoOp> threadA();
        Pthread::Thread<NoOp> threadB();
    }

    void activate()
    {

    }

private:
};

int main()
{
    // Create two threads to signal between.

    return 0;
}
