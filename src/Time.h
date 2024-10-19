#ifndef PUBSUB_TIME_H
#define PUBSUB_TIME_H

#include <mutex>
#include <condition_variable>

namespace PubSub
{
class Time
{
public:
    Time();
    ~Time();
    Time( const Time& );

    void syncTime( const double timeNow );
    void reset();

    double getTimeNow() const;
    unsigned int getCounter() const;

private:
    double timeNow_{0.0};
    unsigned int counter_ = 0u;

    mutable std::mutex mtx;
    mutable std::condition_variable cv;
};
} // namespace PubSub

#endif /* PUBSUB_TIME_H */
