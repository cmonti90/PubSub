
#include "Endpoint.h"

namespace PubSub
{

Endpoint::Endpoint()
    : m_queue_mngr( nullptr )
    , m_active_depth( 10u )
    , m_passive_depth( 1u )
{}


Endpoint::Endpoint( std::shared_ptr< QueueMngr >& queue_mngr )
    : m_queue_mngr( queue_mngr )
{}


Endpoint::~Endpoint()
{
    clearBuffers();
}


void Endpoint::configure( std::shared_ptr< QueueMngr >& queue_mngr )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    m_queue_mngr = queue_mngr;
}


void Endpoint::setPassiveDepth( const unsigned int depth )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    m_passive_depth = depth;
}


void Endpoint::setActiveDepth( const unsigned int depth )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    m_active_depth = depth;
}


void Endpoint::subscribe( const Message* msg, const Message_Type msg_type )
{
    if ( m_queue_mngr )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        m_subscribed_msg.insert( std::make_pair( msg->getMessageName(), msg_type ) );

        lock.unlock();

        m_queue_mngr->subscribe( this, msg->getMessageName() );
    }
}


void Endpoint::unsubscribe( const Message* msg )
{
    if ( m_queue_mngr )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        m_subscribed_msg.erase( msg->getMessageName() );

        lock.unlock();

        m_queue_mngr->unsubscribe( this, msg->getMessageName() );
    }
}


MessageStatus Endpoint::peek( Message_Label& msg_label ) const
{
    std::unique_lock< std::mutex > lock( m_mutex );
    MessageStatus status{FAIL };

    if ( m_passive_msg_buffer.empty() )
    {
        if ( m_active_msg_buffer.empty() )
        {   status = FAIL; }
        else
        {
            msg_label = m_active_msg_buffer.begin()->second->getMessageLabel();
            status = MESSAGE_AVAILABLE;
        }
    }
    else
    {
        msg_label = m_passive_msg_buffer.begin()->second->getMessageLabel();
        status = MESSAGE_AVAILABLE;
    }

    lock.unlock();

    return status;
}


void Endpoint::send( Message* msg ) const
{
    m_queue_mngr->push( msg );
}


void Endpoint::receive( Message* msg )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    switch ( m_subscribed_msg.find( msg->getMessageName() )->second )
    {
        case ACTIVE:
        {
            msg->copy( m_active_msg_buffer.lower_bound( msg->getMessageName() )->second );

            delete m_active_msg_buffer.lower_bound( msg->getMessageName() )->second;

            m_active_msg_buffer.erase( m_active_msg_buffer.lower_bound( msg->getMessageName() ) );
        }
        break;

        case PASSIVE:

            msg->copy( m_passive_msg_buffer.lower_bound( msg->getMessageName() )->second );

            delete m_passive_msg_buffer.lower_bound( msg->getMessageName() )->second;

            m_passive_msg_buffer.erase( m_passive_msg_buffer.lower_bound( msg->getMessageName() ) );

            break;

        default:
            break;
    }
}


void Endpoint::removeTopMessage()
{
    std::unique_lock< std::mutex > lock( m_mutex );

    if ( !m_passive_msg_buffer.empty() )
    {
        delete m_passive_msg_buffer.begin()->second;

        m_passive_msg_buffer.erase( m_passive_msg_buffer.begin() );
    }
    else if ( !m_active_msg_buffer.empty() )
    {
        delete m_active_msg_buffer.begin()->second;

        m_active_msg_buffer.erase( m_active_msg_buffer.begin() );
    }
}


void Endpoint::clear()
{
    clearBuffers();
}


void Endpoint::writeToBuffer( Message* msg )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    switch ( m_subscribed_msg.find( msg->getMessageName() )->second )
    {
        case ACTIVE:
        {
            if ( m_active_msg_buffer.size() >= m_active_depth )
            {
                delete m_active_msg_buffer.begin()->second;

                m_active_msg_buffer.erase( m_active_msg_buffer.begin() );
            }

            writeToBuffer( msg->clone(), m_active_msg_buffer );
        }
        break;

        case PASSIVE:

            if ( m_passive_msg_buffer.size() >= m_passive_depth )
            {
                delete m_passive_msg_buffer.begin()->second;

                m_passive_msg_buffer.erase( m_passive_msg_buffer.begin() );
            }

            writeToBuffer( msg->clone(), m_passive_msg_buffer );
            break;

        default:
            break;
    }
}


void Endpoint::writeToBuffer( Message* msg, MessageBuffer& buffer )
{
    buffer.insert( MessageBuffer::value_type( msg->getMessageName(), msg ) );
}


bool Endpoint::hasActiveMessage() const
{
    std::lock_guard< std::mutex > lock( m_mutex );

    return !m_active_msg_buffer.empty();
}


void Endpoint::finalize()
{
    std::unique_lock< std::mutex > lock( m_mutex );

    clear();

    m_subscribed_msg.clear();
    m_queue_mngr.reset();
}


void Endpoint::clearBuffers()
{
    clearActiveBuffer();
    clearPassiveBuffer();
}


void Endpoint::clearActiveBuffer()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    
    for ( auto& msg : m_active_msg_buffer )
    {
        delete msg.second;
    }

    m_active_msg_buffer.clear();
}


void Endpoint::clearPassiveBuffer()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    
    for ( auto& msg : m_passive_msg_buffer )
    {
        delete msg.second;
    }

    m_passive_msg_buffer.clear();
}


} // namespace PubSub

