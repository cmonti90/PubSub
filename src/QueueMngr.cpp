
#include "QueueMngr.h"
#include "Endpoint.h"

namespace PubSub
{
void QueueMngr::subscribe( Endpoint* endpoint, const Message_Name msgName )
{
    std::unique_lock<std::mutex> lock( m_mutex );

    if ( m_subscriberList.count( msgName ) )
    {

        m_subscriberList.find( msgName )->second.push_back( endpoint );
    }
    else
    {
        m_subscriberList.insert( std::make_pair( msgName, std::list<Endpoint*> {endpoint} ) );
    }

    m_subscriberList[msgName].unique();

    lock.unlock();
}

void QueueMngr::unsubscribe( Endpoint* endpoint, const Message_Name msgName )
{
    std::unique_lock<std::mutex> lock( m_mutex );

    if ( m_subscriberList.count( msgName ) )
    {
        m_subscriberList.find( msgName )->second.remove( endpoint );
    }

    lock.unlock();
}

void QueueMngr::push( const Message* value )
{
    std::unique_lock<std::mutex> lock( m_mutex );

    m_queue.push( value->clone() );

    lock.unlock();
}

Message* QueueMngr::popFront()
{
    Message* value = m_queue.front();
    m_queue.pop();

    return value;
}

void QueueMngr::dispatch()
{
    std::unique_lock<std::mutex> lock( m_mutex );

    while ( !m_queue.empty() )
    {
        Message* msg = popFront();

        for ( EndpointList::iterator it = m_subscriberList[msg->getMessageName()].begin(); it != m_subscriberList[msg->getMessageName()].end(); ++it )
        {
            ( *it )->writeToBuffer( msg );
        }

        delete msg;
    }

    lock.unlock();
}

} // namespace PubSub