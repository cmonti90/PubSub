
#include "Module.h"

namespace PubSub
{

    Module::Module() : m_threadCount{0u}, maxProcCount{0u}
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

    void Module::initialize()
    {
        run(Thread::ThreadState::INITIALIZE);
    }

    void Module::start()
    {
        run(Thread::ThreadState::UPDATE);
    }

    void Module::finalize()
    {
        run(Thread::ThreadState::FINALIZE);
    }

    void Module::run(const Thread::ThreadState &threadState)
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
        }

        for (unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++)
        {
            m_threads[threadIdx].resetProcessCount();
        }
    }

} // namespace PubSub
