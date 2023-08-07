#include "Thread.h"
#include "Time.h"

#include <iostream>

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
        std::cout << "Thread::run() procIdx: " << procIdx << ", m_procs.size() = " << m_procs.size() << ", threadState = " << threadState << std::endl;

        if (procIdx < m_procs.size())
        {
            std::cout << "Thread::run() proc: " << m_procs[procIdx]->getComponentLabel();

            if (threadState == ThreadState::INITIALIZE)
            {
                std::cout << " initialize()" << std::endl;

                thread = std::thread(&Component::initialize, m_procs[procIdx]);
            }
            else if (threadState == ThreadState::UPDATE)
            {
                std::cout << " update()" << std::endl;

                if (m_procs[procIdx]->hasActiveMessage())
                {
                    std::cout << " hasActiveMessage()" << std::endl;

                    thread = std::thread(&Component::update, m_procs[procIdx]);
                }
            }
            else if (threadState == ThreadState::FINALIZE)
            {
                std::cout << " finalize()" << std::endl;

                thread = std::thread(&Component::finalize, m_procs[procIdx]);
            }

            procIdx++;
        }

        std::cout << "Finished Thread::run()" << std::endl;
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
        std::cout << "Thread::joinable()" << std::endl;

        if (thread.joinable())
        {
            std::cout << "Thread::join()" << std::endl;
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