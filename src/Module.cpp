
#include "Module.h"

#include <iostream>
namespace PubSub
{

    Module::Module() : m_threadCount(0u), maxProcCount(0u), m_queueMngr(new QueueMngr()), m_time()
    {
    }

    void Module::addThread(Thread &thread)
    {
        m_threads.emplace_back(std::move(thread));
        m_threadCount++;
    }

    void Module::addCompToThread(Component *comp)
    {
        if (!m_threads.empty())
        {
            (m_threads.end() - 1)->addComp(comp);

            maxProcCount = maxProcCount < (m_threads.end() - 1)->getProcessCount() ? (m_threads.end() - 1)->getProcessCount() : maxProcCount;
        }
    }

    void Module::addSimComp(SimComponent *comp)
    {
        m_simThread.addComp(comp);
    }

    void Module::initialize()
    {
        std::cout << "MODULE: initialize()" << std::endl;
        run(Thread::ThreadState::INITIALIZE);

        passSubscriptionLists();
    }

    void Module::start()
    {
        run(Thread::ThreadState::UPDATE);
    }

    void Module::stop(bool over_ride)
    {
        for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
        {
            m_threads[threadIdx].stop();
        }

        m_simThread.stop();

        if (!over_ride)
        {
            finalize();

            for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
            {
                m_threads[threadIdx].stop();
            }

            m_simThread.stop();
        }
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

        m_simThread.passSubscriptionLists();
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
            std::cout << "MODULE: Running SW threads" << std::endl;

            for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
            {
                m_threads[threadIdx].run(threadState);
            }

            for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
            {
                m_threads[threadIdx].join();
            }

            dispatchMessages(threadState);
        }

        std::cout << "MODULE: Resetting Process Count" << std::endl;
        for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
        {
            m_threads[threadIdx].resetProcessCount();
        }
    }

    void Module::runSim(const Thread::ThreadState &threadState)
    {
        std::cout << "MODULE: Running Sim" << std::endl;
        for (unsigned int procIdx{0u}; procIdx < m_simThread.getProcessCount(); procIdx++)
        {
            std::cout << "MODULE: Running Sim Thread" << std::endl;
            m_simThread.run(threadState, m_time.getCounter());

            dispatchMessages(threadState);
        }

        m_simThread.resetProcessCount();
    }

    void Module::dispatchMessages(const Thread::ThreadState &threadState)
    {
        if (threadState == Thread::ThreadState::UPDATE)
        {
            m_queueMngr->dispatch();
        }
    }

} // namespace PubSub
