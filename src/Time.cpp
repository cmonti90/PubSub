
#include "Time.h"

namespace PubSub
{

    void Time::incrementTime()
    {
        timeNow += 0.1;

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