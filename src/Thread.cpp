#include "Thread.h"

namespace PubSub
{

    Thread::Thread() : procIdx{0u}
    {
    }

    Thread::~Thread()
    {
    }

    Thread::Thread(Thread &&obj) : procIdx{obj.procIdx}, m_procs{std::move(obj.m_procs)}, thread{std::move(obj.thread)}
    {
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
                thread = std::thread(&Component::update, m_procs[procIdx]);
            }
            else if (threadState == ThreadState::FINALIZE)
            {
                thread = std::thread(&Component::finalize, m_procs[procIdx]);
            }

            procIdx++;
        }
    }

    void Thread::join()
    {
        thread.join();
    }

    void Thread::addComp(Component *comp)
    {
        m_procs.push_back(comp);
    }

} // namespace PubSub