#include "Thread.h"
#include "Time.h"

namespace PubSub
{

    Thread::Thread() : procIdx(0u), m_procs(), thread()
    {
    }

    Thread::~Thread()
    {
    }

    Thread::Thread(Thread &&obj) : procIdx(obj.procIdx), m_procs(std::move(obj.m_procs)), thread(std::move(obj.thread))
    {
    }

    Thread &Thread::operator=(Thread &&obj)
    {
        if (thread.joinable())
        {
            thread.join();
        }

        procIdx = obj.procIdx;
        m_procs = std::move(obj.m_procs);
        thread = std::move(obj.thread);

        return *this;
    }

    void Thread::run(const ThreadState &threadState)
    {
        if (procIdx < m_procs.size())
        {
            if (threadState == ThreadState::INITIALIZE)
            {
                thread = std::thread(&Component::initialize, m_procs[procIdx]);
            }
            else if (threadState == ThreadState::UPDATE)
            {
                if (m_procs[procIdx]->hasActiveMessage())
                {
                    thread = std::thread(&Component::update, m_procs[procIdx]);
                }
            }
            else if (threadState == ThreadState::FINALIZE)
            {
                thread = std::thread(&Component::finalize, m_procs[procIdx]);
            }

            procIdx++;
        }
    }

    void Thread::stop()
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    void Thread::join()
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    void Thread::addComp(Component *comp)
    {
        m_procs.push_back(comp);
    }

    void Thread::passSubscriptionLists()
    {
        for (unsigned int i{0u}; i < m_procs.size(); i++)
        {
            thread = std::thread(&Component::giveSubscriptionListToQueueMngr, m_procs[i]);

            join();
        }
    }

} // namespace PubSub