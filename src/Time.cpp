
#include "Time.h"

namespace PubSub
{
Time::Time()
    : timeNow_( 0.0 )
    , counter_( 0u )
    , mtx()
    , cv()
{
}

Time::~Time()
{
}

Time::Time( const Time& obj )
    : timeNow_( obj.timeNow_ )
    , counter_( obj.counter_ )
    , mtx()
    , cv()
{
}

void Time::syncTime( const double timeNow )
{
    std::unique_lock< std::mutex > lck( mtx );

    timeNow_ = timeNow;

    lck.unlock();
}

void Time::reset()
{
    std::unique_lock< std::mutex > lck( mtx );

    timeNow_ = 0.0;
    counter_ = 0u;

    lck.unlock();
}

double Time::getTimeNow() const
{
    std::lock_guard< std::mutex > lck( mtx );

    return timeNow_;
}

unsigned int Time::getCounter() const
{
    std::lock_guard< std::mutex > lck( mtx );

    return counter_;
}

} // namespace PubSub