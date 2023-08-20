#ifndef D7319A1C_DBBF_4452_A940_C7793A94A396
#define D7319A1C_DBBF_4452_A940_C7793A94A396

#include <mutex>
#include <condition_variable>

namespace PubSub
{
    class Time
    {
    public:
        Time();
        ~Time() = default;
        Time(const Time &);

        static constexpr unsigned int SimulationRunRate = 1000u;

        void incrementTime();
        void finalize();
        void reset();

        double getTimeNow() const;
        unsigned int getCounter() const;

    private:
        double timeNow{0.0};
        unsigned int counter = 0u;
        
        mutable std::mutex mtx;
        mutable std::condition_variable cv;
    };
} // namespace PubSub

#endif /* D7319A1C_DBBF_4452_A940_C7793A94A396 */
