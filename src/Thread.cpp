#include "Thread.h"
#include "Time.h"

namespace PubSub
{

Thread::Thread( const ThreadName& name )
    : mtx()
    , m_procs()
    , m_threadName( name )
{
}

Thread::~Thread()
{
}

Thread::Thread( Thread& obj )
    : mtx()
    , m_procs( obj.m_procs )
    , m_threadName( obj.GetThreadName() )
{
}

void Thread::Run( const ThreadState& threadState )
{
    std::unique_lock<std::mutex> lck( mtx );

    if ( threadState == ThreadState::INITIALIZE )
    {
        RunInitialize();
    }
    else if ( threadState == ThreadState::UPDATE )
    {
        RunUpdate();
    }
    else if ( threadState == ThreadState::FINALIZE )
    {
        RunFinalize();
    }
    else
    {
        std::runtime_error( "Thread state not recognized!" );
    }

    lck.unlock();
}


void Thread::RunInitialize()
{
    for ( unsigned int procIdx = 0u; procIdx < m_procs.size(); procIdx++ )
    {
        m_procs[procIdx]->initialize();
    }
}

void Thread::RunUpdate()
{
    for ( unsigned int procIdx = 0u; procIdx < m_procs.size(); procIdx++ )
    {
        if ( m_procs[procIdx]->associateEvent() )
        {
            m_procs[procIdx]->update();
        }
    }
}

void Thread::RunFinalize()
{
    for ( unsigned int procIdx = 0u; procIdx < m_procs.size(); procIdx++ )
    {
        m_procs[procIdx]->finalize();
    }
}

void Thread::AddComp( Component* comp )
{
    std::unique_lock<std::mutex> lck( mtx );

    for ( unsigned int procIdx = 0u; procIdx < m_procs.size(); procIdx++ )
    {
        if ( m_procs[procIdx] == comp )
        {
            std::runtime_error( "Component already added to thread!" );
        }
    }

    m_procs.push_back( comp );

    lck.unlock();
}

ThreadName Thread::GetThreadName() const
{
    return m_threadName;
}

} // namespace PubSub