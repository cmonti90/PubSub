#ifndef D7319A1C_DBBF_4452_A940_C7793A94A396
#define D7319A1C_DBBF_4452_A940_C7793A94A396

namespace PubSub
{
    class Time
    {
    public:
        Time() = default;
        ~Time() = default;

        void incrementTime();
        void finalize();
        void reset();

        double getTimeNow() const;
        unsigned int getCounter() const;

    private:
        double timeNow{0.0};
        unsigned int counter;
    };
} // namespace PubSub

#endif /* D7319A1C_DBBF_4452_A940_C7793A94A396 */
