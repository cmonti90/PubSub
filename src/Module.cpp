
#include "Module.h"

namespace PubSub
{

    void Module::addThread(Thread &thread)
    {
        m_threads.push_back(thread);
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

    void Module::start()
    {
        for (unsigned int procIdx{0u}; procIdx < maxProcCount; procIdx++)
        {
            for (auto &thread : m_threads)
            {
                if (procIdx < thread.getProcessCount())
                {
                    thread.start();
                }
            }

            for (auto &thread : m_threads)
            {
                thread.join();
            }
        }

        for (auto &thread : m_threads)
        {
            thread.resetProcessCount();
        }
    }

    void Module::stop()
    {
        for (auto &thread : m_threads)
        {
            thread.stop();
        }
    }
} // namespace PubSub
