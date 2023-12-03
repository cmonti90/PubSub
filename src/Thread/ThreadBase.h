#ifndef B9448F8A_9F06_44EE_8647_8717E632DB74
#define B9448F8A_9F06_44EE_8647_8717E632DB74

#include <thread>
#include <mutex>
#include <condition_variable>

namespace PubSub
{
    static const unsigned int MAX_THREAD_COUNT = 8u;
    static unsigned int NUM_THREADS_ACTIVE = 0u;

    class ThreadBase
    {
      public:
        enum ThreadState
        {
            INITIALIZE,
            UPDATE,
            FINALIZE
        };

        ThreadBase();
        virtual ~ThreadBase();
        ThreadBase( ThreadBase &&obj );

        ThreadBase& operator=( ThreadBase &&obj );

        void stop();
        void join();

        virtual unsigned int getProcessCount() const = 0;
        void resetProcessCount() {
            procIdx = 0u;
        }

      protected:
        unsigned int procIdx;
        std::thread thread;

        std::mutex mtx;
        std::condition_variable cv;

      private:
        ThreadBase( const ThreadBase& ) = delete;
        ThreadBase& operator=( const ThreadBase& ) = delete;
    };

} // namespace PubSub


#endif /* B9448F8A_9F06_44EE_8647_8717E632DB74 */
