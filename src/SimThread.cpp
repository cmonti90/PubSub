#include "SimThread.h"
#include "Time.h"
#include <iostream>

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
        std::cout << "Running sim thread" << std::endl;
        if (threadState == ThreadState::INITIALIZE)
        {
            std::cout << "Running sim thread initialize" << std::endl;
            thread = std::thread(&SimComponent::initialize, m_simProcs[procIdx]);
        }
        else if (threadState == ThreadState::UPDATE)
        {
            std::cout << "Running sim thread update" << std::endl;
            if ((counter % static_cast<unsigned int>(m_simProcs[procIdx]->getModelRate() / Time::SimulationRunRate)) == 0)
            {

                std::cout << "Running " << m_simProcs[procIdx]->getComponentLabel() << " sim thread update" << std::endl;
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

} // namespace PubSub