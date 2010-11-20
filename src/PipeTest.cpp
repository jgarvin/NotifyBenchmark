#include <utility>

#include "Pthreads/Thread.hpp"
#include "Pthreads/Condition.hpp"

struct NoOp
{
    typedef int Data;
    static void action(int) {}
};

template<class RealCallback>
struct WaitForCondition
{
    typedef std::pair<Pthread::Condition&, typename RealCallback::Data> Data;
    static void action(Data& cond_data_pair)
    {
        cond_data_pair.first.wait();
        RealCallback::action(cond_data_pair.second);
    }
};

// FIXME: Later can lift things out of the class, define there for now.
class PipeTest : boost::noncopyable
{
public:
    PipeTest()
    {
        Pthread::Condition start_condition;

        typedef WaitForCondition<NoOp> DelayedNoOp;

        Pthread::Thread< DelayedNoOp > threadA(DelayedNoOp::Data(start_condition, 0));
        Pthread::Thread< DelayedNoOp > threadB(DelayedNoOp::Data(start_condition, 0));
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
