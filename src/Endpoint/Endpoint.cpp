
#include "Endpoint.h"

namespace PubSub
{
    Endpoint::Endpoint( std::shared_ptr<QueueMngr>& queue_mngr )
        : m_queue_mngr( queue_mngr )
        , m_component ( nullptr )
    {
    }

    Endpoint::~Endpoint()
    {
    }

    void Endpoint::associate( Component* comp )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        m_component.reset( comp );

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::subscribe( const Message* msg, const Message_Type msg_type )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        m_subscribed_msg.insert( std::make_pair( msg->getMessageName(), msg_type ) );

        m_queue_mngr->subscribe( this, msg->getMessageName() );

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::unsubscribe( const Message* msg )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        m_subscribed_msg.erase( msg->getMessageName() );

        lock.unlock();
        m_condition.notify_one();
    }

    MessageStatus Endpoint::peek( Message_Label& msg_label ) const
    {
        std::unique_lock<std::mutex> lock( m_mutex );
        MessageStatus status{FAIL};

        if ( m_passive_msg_buffer.empty() )
        {
            if ( m_active_msg_buffer.empty() )
            {
                status = FAIL;
            }
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
        m_condition.notify_one();

        return status;
    }

    void Endpoint::send( Message* msg ) const
    {
        m_queue_mngr->push( msg );
    }

    void Endpoint::receive( Message* msg )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        switch ( m_subscribed_msg.find( msg->getMessageName() )->second )
        {
            case ACTIVE:
                {
                    msg->copy( m_active_msg_buffer.lower_bound( msg->getMessageName() )->second.get() );

                    m_active_msg_buffer.erase( m_active_msg_buffer.lower_bound( msg->getMessageName() ) );
                }
                break;

            case PASSIVE:
                {
                    msg->copy( m_passive_msg_buffer.lower_bound( msg->getMessageName() )->second.get() );

                    m_passive_msg_buffer.erase( m_passive_msg_buffer.lower_bound( msg->getMessageName() ) );
                }
                break;
            default:
                break;
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::removeTopMessage()
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        if ( !m_passive_msg_buffer.empty() )
        {
            m_passive_msg_buffer.erase( m_passive_msg_buffer.begin() );
        }
        else if ( !m_active_msg_buffer.empty() )
        {
            m_active_msg_buffer.erase( m_active_msg_buffer.begin() );
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::clear()
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        m_active_msg_buffer.clear();
        m_passive_msg_buffer.clear();

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::writeToBuffer( Message* msg )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        
        switch (m_subscribed_msg.find(msg->getMessageName())->second)
        {

        case ACTIVE:
        {
            writeToBuffer(msg, m_active_msg_buffer);
        }
        break;

        case PASSIVE:
        {
            writeToBuffer(msg, m_passive_msg_buffer);
        }
        break;

        default:
            break;
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::writeToBuffer( Message* msg, MessageBuffer& buffer )
    {
        buffer.insert( MessageBuffer::value_type( msg->getMessageName(), std::unique_ptr<Message>( msg->clone() ) ) );
    }

    bool Endpoint::hasActiveMessage() const
    {
        std::lock_guard<std::mutex> lock( m_mutex );

        return !m_active_msg_buffer.empty();
    }

} // namespace PubSub

