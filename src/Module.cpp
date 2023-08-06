
#include "Module.h"

namespace PubSub
{

    Module::Module() : m_threadCount(0u), maxProcCount(0u), m_queueMngr()
    {
    }

    void Module::addThread(Thread &thread)
    {
        m_threads.emplace_back(std::move(thread));
        m_threadCount++;
    }

    void Module::addCompToThread(Thread &thread, Component *comp)
    {
        for (unsigned int i{0u}; i < m_threads.size(); i++)
        {
            if (&(m_threads[i]) == &thread)
            {
                m_threads[i].addComp(comp);

                maxProcCount = maxProcCount < m_threads[i].getProcessCount() ? m_threads[i].getProcessCount() : maxProcCount;

                break;
            }
        }
    }

    void Module::addSimComp(SimComponent *comp)
    {
        m_simThread.push_back(comp);
    }

    void Module::initialize()
    {
        run(Thread::ThreadState::INITIALIZE);

        passSubscriptionLists();
    }

    void Module::start()
    {
        run(Thread::ThreadState::UPDATE);
    }

    void Module::finalize()
    {
        run(Thread::ThreadState::FINALIZE);
    }

    void Module::passSubscriptionLists()
    {
        for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
        {
            m_threads[threadIdx].passSubscriptionLists();
        }

        for (unsigned int threadIdx{0u}; threadIdx < m_simThread.size(); threadIdx++)
        {
            m_simThread[threadIdx].passSubscriptionLists();
        }
    }

    void Module::run(const Thread::ThreadState &threadState)
    {
        runSW(threadState);

        m_time.incrementTime();

        runSim(threadState);
    }

    void Module::runSW(const Thread::ThreadState &threadState)
    {
        for (unsigned int procIdx{0u}; procIdx < maxProcCount; procIdx++)
        {
            for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
            {
                m_threads[threadIdx].run(threadState);
            }

            for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
            {
                m_threads[threadIdx].join();
            }

            m_queueMngr.dispatch();
        }

        for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
        {
            m_threads[threadIdx].resetProcessCount();
        }
    }

    void Module::runSim(const Thread::ThreadState &threadState)
    {
        for (unsigned int procIdx{0u}; procIdx < m_simThread.getProcessCount(); procIdx++)
        {
            m_simThread.runSingular(threadState, m_time.getCounter());
            m_queueMngr.dispatch();
        }

        m_simThread.resetProcessCount();
    }

} // namespace PubSub
