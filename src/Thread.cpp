#include "Thread.h"

namespace PubSub
{

    void Thread::start()
    {
        thread = std::thread(&Component::update, m_procs[procIdx]);

        procIdx++;
    }

    void Thread::join()
    {
        thread.join();
    }

    void Thread::stop()
    {
        thread.join();
    }

    void Thread::addComp(Component *comp)
    {
        m_procs.push_back(comp);
    }

} // namespace PubSub