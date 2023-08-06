#ifndef DC26027A_04B1_4A1D_B196_A076A42F9DFD
#define DC26027A_04B1_4A1D_B196_A076A42F9DFD

#include "Component.h"
#include "SimComponent.h"
#include "Thread.h"
#include "QueueMngr.h"
#include "Time.h"
#include <thread>
#include <vector>

namespace PubSub
{
    typedef std::vector<Thread> ThreadList;
    class Module
    {
    public:
        Module();
        ~Module() = default;

        void addThread(Thread &thread);
        void addCompToThread(Thread &thread, Component *comp);

        void addSimComp(SimComponent *comp);

        void initialize();
        void start();
        void finalize();

        void run(const Thread::ThreadState &threadState);

    private:
        unsigned int m_threadCount{0u};
        unsigned int maxProcCount{0u};

        ThreadList m_threads;

        std::vector<Thread> m_simThread;

        QueueMngr m_queueMngr;

        Time m_time;

        void runSW(const Thread::ThreadState &threadState);
        void runSim(const Thread::ThreadState &threadState);
        void passSubscriptionLists();

        Module(const Module &) = delete;
        Module &operator=(const Module &) = delete;

    };
} // namespace PubSub

#endif /* DC26027A_04B1_4A1D_B196_A076A42F9DFD */
