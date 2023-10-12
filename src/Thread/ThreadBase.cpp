#include "ThreadBase.h"
#include "Time.h"

namespace PubSub
{

    ThreadBase::ThreadBase() : procIdx( 0u ), thread()
    {
    }

    ThreadBase::~ThreadBase()
    {
    }

    ThreadBase::ThreadBase( ThreadBase&& obj ) : procIdx( obj.procIdx ), thread( std::move( obj.thread ) )
    {
    }

    ThreadBase& ThreadBase::operator=( ThreadBase&& obj )
    {
        if ( thread.joinable() )
        {
            thread.join();
        }

        procIdx = obj.procIdx;
        thread = std::move( obj.thread );

        return *this;
    }

    void ThreadBase::stop()
    {
        join();
    }

    void ThreadBase::join()
    {
        if ( thread.joinable() )
        {
            thread.join();
        }

        NUM_THREADS_ACTIVE--;
    }

} // namespace PubSub