
#include "Time.h"

namespace PubSub
{
    Time::Time() : timeNow(0.0), counter(0u), mtx(), cv()
    {
    }

    Time::Time(const Time &obj) : timeNow(obj.timeNow), counter(obj.counter), mtx(), cv()
    {
    }

    void Time::incrementTime()
    {
        std::unique_lock<std::mutex> lck(mtx);

        timeNow += 1.0 / static_cast<double>(SimulationRunRate);

        counter++;

        cv.notify_one();
    }

    void Time::reset()
    {
        timeNow = 0.0;
        counter = 0u;
    }

    void Time::finalize()
    {
    }

    double Time::getTimeNow() const
    {
        std::unique_lock<std::mutex> lck(mtx);

        double timeNowTmp = timeNow;

        lck.unlock();
        cv.notify_one();

        return timeNowTmp;
    }

    unsigned int Time::getCounter() const
    {
        std::unique_lock<std::mutex> lck(mtx);


        double counterTmp = counter;

        lck.unlock();
        cv.notify_one();

        return counterTmp;
    }

} // namespace PubSub