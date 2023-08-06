
#include "Time.h"

namespace PubSub
{

    void Time::incrementTime()
    {
        timeNow += 1.0 / static_cast<double>(SimulationRunRate);

        counter++;
    }

    void Time::reset()
    {
        timeNow = 0.0;
        counter = 0u;
    }

    void Time::finalize()
    {
    }

} // namespace PubSub