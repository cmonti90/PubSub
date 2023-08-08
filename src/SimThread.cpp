#include "SimThread.h"
#include "Time.h"

namespace PubSub
{

    SimThread::SimThread() : Thread()
    {
    }

    SimThread::~SimThread()
    {
    }

    SimThread::SimThread(SimThread &&obj) : Thread(std::move(obj)), m_simProcs(std::move(obj.m_simProcs))
    {
    }

    SimThread &SimThread::operator=(SimThread &&obj)
    {
        if (thread.joinable())
        {
            thread.join();
        }

        procIdx = obj.procIdx;
        m_simProcs = std::move(obj.m_simProcs);
        thread = std::move(obj.thread);

        return *this;
    }

    void SimThread::run(const ThreadState &threadState, unsigned int counter)
    {
        if (threadState == ThreadState::INITIALIZE)
        {
            thread = std::thread(&SimComponent::initialize, m_simProcs[procIdx]);
        }
        else if (threadState == ThreadState::UPDATE)
        {
            if ((counter % static_cast<unsigned int>(m_simProcs[procIdx]->getModelRate() / Time::SimulationRunRate)) == 0)
            {
                thread = std::thread(&SimComponent::update, m_simProcs[procIdx]);
            }
        }
        else if (threadState == ThreadState::FINALIZE)
        {
            thread = std::thread(&SimComponent::finalize, m_simProcs[procIdx]);
        }

        join();

        procIdx++;
    }

    void SimThread::addComp(SimComponent *comp)
    {
        m_simProcs.push_back(comp);
    }

    void SimThread::passSubscriptionLists()
    {
        for (unsigned int i{0u}; i < m_simProcs.size(); i++)
        {
            thread = std::thread(&Component::giveSubscriptionListToQueueMngr, m_simProcs[i]);

            join();
        }
    }

} // namespace PubSub