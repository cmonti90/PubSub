#ifndef DC26027A_04B1_4A1D_B196_A076A42F9DFD
#define DC26027A_04B1_4A1D_B196_A076A42F9DFD

#include "Component.h"
#include "Thread.h"
#include <thread>
#include <vector>

namespace PubSub
{
    class Module
    {
    public:
        Module() = default;
        ~Module() = default;

        void addThread(Thread &thread);
        void addCompToThread(Thread &thread, Component *comp);

        void start();
        void stop();

    private:
        unsigned int m_threadCount{0u};
        unsigned int maxProcCount{0u};

        std::vector<Thread> m_threads;
    };
} // namespace PubSub

#endif /* DC26027A_04B1_4A1D_B196_A076A42F9DFD */
