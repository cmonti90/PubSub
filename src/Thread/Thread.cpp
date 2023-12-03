#include "Thread.h"
#include "Time.h"

namespace PubSub
{

    Thread::Thread() : ThreadBase(), m_procs()
    {
    }

    Thread::~Thread()
    {
    }

    Thread::Thread( Thread &&obj ) : ThreadBase( std::move( obj ) ), m_procs( std::move( obj.m_procs ) )
    {
    }

    Thread& Thread::operator=( Thread &&obj )
    {
        if ( thread.joinable() )
        {
            thread.join();
        }

        procIdx = obj.procIdx;
        m_procs = std::move( obj.m_procs );
        thread = std::move( obj.thread );

        return *this;
    }

    void Thread::run( const ThreadState& threadState )
    {
        if ( procIdx < m_procs.size() )
        {

            // while (NUM_THREADS_ACTIVE >= MAX_THREAD_COUNT)
            // {
            //     join();
            // }

            if ( threadState == ThreadState::INITIALIZE )
            {
                thread = std::thread( &Component::initialize, m_procs[procIdx] );
            }
            else if ( threadState == ThreadState::UPDATE )
            {
                if ( m_procs[procIdx]->associateEvent() )
                {
                    thread = std::thread( &Component::update, m_procs[procIdx] );
                }
            }
            else if ( threadState == ThreadState::FINALIZE )
            {
                thread = std::thread( &Component::finalize, m_procs[procIdx] );
            }

            procIdx++;

            NUM_THREADS_ACTIVE++;
        }
    }

    void Thread::addComp( Component* comp )
    {
        m_procs.push_back( comp );
    }

} // namespace PubSub