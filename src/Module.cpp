
#include "Module.h"

namespace PubSub
{

    Module::Module() : m_threadCount( 0u ), maxProcCount( 0u ), m_queueMngr( new QueueMngr() ), m_time( new Time() )
    {
    }

    Module::Module( const std::shared_ptr< QueueMngr >& queueMngr) : m_threadCount( 0u ), maxProcCount( 0u ), m_queueMngr( queueMngr ), m_time( new Time() )
    {
    }

    void Module::addThread( Thread& thread )
    {
        m_threads.emplace_back( std::move( thread ) );
        m_threadCount++;
    }

    void Module::addCompToThread( Component* comp )
    {
        if ( !m_threads.empty() )
        {
            ( m_threads.end() - 1 )->addComp( comp );

            maxProcCount = maxProcCount < ( m_threads.end() - 1 )->getProcessCount() ? ( m_threads.end() - 1 )->getProcessCount() : maxProcCount;
        }
    }

    void Module::initialize()
    {
        run( ThreadBase::ThreadState::INITIALIZE );
    }

    void Module::iterate()
    {
        run( ThreadBase::ThreadState::UPDATE );
    }

    void Module::stop( bool over_ride )
    {
        for ( unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++ )
        {
            m_threads[threadIdx].stop();
        }

        if ( !over_ride )
        {
            finalize();

            for ( unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++ )
            {
                m_threads[threadIdx].stop();
            }
        }
    }

    void Module::finalize()
    {
        run( ThreadBase::ThreadState::FINALIZE );
    }

    void Module::run( const ThreadBase::ThreadState& threadState )
    {
        runSW( threadState );

        m_time->incrementTime();
    }

    void Module::runSW( const ThreadBase::ThreadState& threadState )
    {
        for ( unsigned int procIdx{0u}; procIdx < maxProcCount; procIdx++ )
        {

            for ( unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++ )
            {
                m_threads[threadIdx].run( threadState );
            }

            for ( unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++ )
            {
                m_threads[threadIdx].join();
            }
        }

        for ( unsigned int threadIdx{0u}; threadIdx < m_threads.size(); threadIdx++ )
        {
            m_threads[threadIdx].resetProcessCount();
        }
    }
} // namespace PubSub
