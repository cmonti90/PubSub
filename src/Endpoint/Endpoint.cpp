
#include "Endpoint.h"

namespace PubSub
{
    Endpoint::Endpoint()
        : m_queue_mngr( nullptr )
    {
    }

    Endpoint::Endpoint( std::shared_ptr<QueueMngr>& queue_mngr )
        : m_queue_mngr( queue_mngr )
    {
    }

    Endpoint::~Endpoint()
    {
    }

    void Endpoint::configure( std::shared_ptr<QueueMngr>& queue_mngr )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        m_queue_mngr = queue_mngr;

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::associate( Component* comp )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::subscribe( const Message* msg, const Message_Type msg_type )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        if ( m_queue_mngr )
        {
            m_subscribed_msg.insert( std::make_pair( msg->getMessageName(), msg_type ) );

            m_queue_mngr->subscribe( this, msg->getMessageName() );
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Endpoint::unsubscribe( const Message* msg )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        if ( m_queue_mngr )
        {
            m_subscribed_msg.erase( msg->getMessageName() );

            m_queue_mngr->unsubscribe( this, msg->getMessageName() );
        }

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
                    msg->copy( m_active_msg_buffer.lower_bound( msg->getMessageName() )->second );

                    delete m_active_msg_buffer.lower_bound( msg->getMessageName() )->second;

                    m_active_msg_buffer.erase( m_active_msg_buffer.lower_bound( msg->getMessageName() ) );
                }
                break;

            case PASSIVE:
                {
                    msg->copy( m_passive_msg_buffer.lower_bound( msg->getMessageName() )->second );

                    delete m_passive_msg_buffer.lower_bound( msg->getMessageName() )->second;

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
            delete m_passive_msg_buffer.begin()->second;

            m_passive_msg_buffer.erase( m_passive_msg_buffer.begin() );
        }
        else if ( !m_active_msg_buffer.empty() )
        {
            delete m_active_msg_buffer.begin()->second;

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


        switch ( m_subscribed_msg.find( msg->getMessageName() )->second )
        {
            case ACTIVE:
                {
                    writeToBuffer( msg->clone(), m_active_msg_buffer );
                }
                break;

            case PASSIVE:
                {
                    writeToBuffer( msg->clone(), m_passive_msg_buffer );
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
        buffer.insert( MessageBuffer::value_type( msg->getMessageName(), msg ) );
    }

    bool Endpoint::hasActiveMessage() const
    {
        std::lock_guard<std::mutex> lock( m_mutex );

        return !m_active_msg_buffer.empty();
    }

} // namespace PubSub

