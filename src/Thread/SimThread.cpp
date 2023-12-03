#include "SimThread.h"
#include "Time.h"

namespace PubSub
{

    SimThread::SimThread() : ThreadBase(), m_simProcs()
    {
    }

    SimThread::~SimThread()
    {
    }

    SimThread::SimThread( SimThread &&obj )
        : ThreadBase( std::move( obj ) ), m_simProcs( std::move( obj.m_simProcs ) )
    {
    }

    SimThread& SimThread::operator=( SimThread &&obj )
    {
        stop();

        procIdx = obj.procIdx;
        m_simProcs = std::move( obj.m_simProcs );
        thread = std::move( obj.thread );

        return *this;
    }

    void SimThread::run( const ThreadState& threadState, unsigned int counter )
    {
        if ( threadState == ThreadState::INITIALIZE )
        {
            thread = std::thread( &SimComponent::initialize, m_simProcs[procIdx] );
        }
        else if ( threadState == ThreadState::UPDATE )
        {
            if ( ( counter % static_cast<unsigned int>( Time::SimulationRunRate / static_cast<unsigned int>( m_simProcs[procIdx]->getModelRate() ) ) ) == 0 )
            {
                thread = std::thread( &SimComponent::update, m_simProcs[procIdx] );
            }
        }
        else if ( threadState == ThreadState::FINALIZE )
        {
            thread = std::thread( &SimComponent::finalize, m_simProcs[procIdx] );
        }

        NUM_THREADS_ACTIVE++;

        // while (NUM_THREADS_ACTIVE >= MAX_THREAD_COUNT)
        // {
        //     std::this_thread::yield();
        // }

        join();

        procIdx++;
    }

    void SimThread::addComp( SimComponent* comp )
    {
        m_simProcs.push_back( comp );
    }

} // namespace PubSub