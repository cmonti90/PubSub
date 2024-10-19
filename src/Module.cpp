
#include "Module.h"

namespace PubSub
{

Module::Module()
    : m_numThreadsActive( 0u )
    , m_queueMngr( new QueueMngr() )
    , m_time( new Time() )
{
}

Module::Module( const std::shared_ptr< QueueMngr >& queueMngr )
    : m_numThreadsActive( 0u )
    , m_queueMngr( queueMngr )
    , m_time( new Time() )
{
}

void Module::RegisterThread( Thread& thread )
{
    if ( m_threads.contains( thread.GetThreadName() ) )
    {
        std::runtime_error( "Thread: " + thread.GetThreadName() + " already registered!" );
    }

    m_threads.insert( { thread.GetThreadName(), &thread } );
}

void Module::AddCompToThread( Thread& thread, Component* comp )
{
    m_threads.at( thread.GetThreadName() )->AddComp( comp );
}

void Module::initialize()
{
    Run( Thread::ThreadState::INITIALIZE );
}

void Module::iterate()
{
    Run( Thread::ThreadState::UPDATE );
}

void Module::finalize()
{
    Run( Thread::ThreadState::FINALIZE );
}

void Module::stop( bool over_ride )
{
    JoinAllThreads();

    if ( !over_ride )
    {
        finalize();

        JoinAllThreads();
    }
}

void Module::Run( const Thread::ThreadState& threadState )
{
    KickoffThreads( threadState );

    #ifndef SIMULATION
    JoinAllThreads();
    #endif
}

void Module::KickoffThreads( const Thread::ThreadState& threadState )
{
    for ( ThreadList::iterator thisThread = m_threads.begin(); thisThread != m_threads.end(); thisThread++ )
    {
        RunThread( thisThread->second, threadState );

        #ifdef SIMULATION
        JoinThread( thisThread->second );
        #endif
    }
}

void Module::RunThread( Thread* thread, const Thread::ThreadState& threadState )
{
    thread->Run( threadState );

    m_numThreadsActive++;

    #ifdef SIMULATION
    JoinThread( thread );
    #endif
}

void Module::JoinThread( Thread* thread )
{
    // thread->Join();
}

void Module::JoinAllThreads()
{
    for ( ThreadList::iterator thisThread = m_threads.begin(); thisThread != m_threads.end(); thisThread++ )
    {
        JoinThread( thisThread->second );
    }
}
} // namespace PubSub
