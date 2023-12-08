
#include "Time.h"

namespace PubSub
{
    Time::Time() : timeNow( 0.0 ), counter( 0u ), mtx(), cv()
    {
    }

    Time::Time( const Time& obj ) : timeNow( obj.timeNow ), counter( obj.counter ), mtx(), cv()
    {
    }

    void Time::incrementTime()
    {
        std::unique_lock<std::mutex> lck( mtx );

        timeNow += 1.0 / static_cast<double>( SimulationRunRate );

        counter++;

        lck.unlock();
        cv.notify_one();
    }

    void Time::reset()
    {
        std::unique_lock<std::mutex> lck( mtx );

        timeNow = 0.0;
        counter = 0u;

        lck.unlock();
        cv.notify_one();
    }

    void Time::finalize()
    {
    }

    double Time::getTimeNow() const
    {
        std::lock_guard<std::mutex> lck( mtx );

        return timeNow;
    }

    unsigned int Time::getCounter() const
    {
        std::lock_guard<std::mutex> lck( mtx );

        return counter;
    }

} // namespace PubSub