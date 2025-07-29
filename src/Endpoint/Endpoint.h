#ifndef PUBSUB_ENDPOINT_H
#define PUBSUB_ENDPOINT_H

#include "Message.h"
#include "Component.h"

#include <memory>

namespace PubSub
{
class Endpoint
{
public:
    typedef std::unordered_map< Message_Name, Message_Type > MessageSubscriptionList;

    Endpoint();
    Endpoint( std::shared_ptr< QueueMngr >& queue_mngr );
    ~Endpoint();

    void configure( std::shared_ptr< QueueMngr >& queue_mngr );

    void setPassiveDepth( const unsigned int depth );
    void setActiveDepth( const unsigned int depth );

    void subscribe( const Message* msg, const Message_Type msg_type = Message_Type::ACTIVE );
    void unsubscribe( const Message* msg );

    MessageStatus peek( Message_Label& msg_label ) const;
    void send( Message* msg ) const;
    void receive( Message* msg );

    void removeTopMessage();
    void clear();

    bool hasActiveMessage() const;

    void finalize();

private:

    friend class QueueMngr;

    void clearBuffers();
    void clearActiveBuffer();
    void clearPassiveBuffer();

    void writeToBuffer( Message* msg );
    void writeToBuffer( Message* msg, MessageBuffer& buffer );

    unsigned int m_active_depth;
    unsigned int m_passive_depth;

    MessageBuffer m_active_msg_buffer;
    MessageBuffer m_passive_msg_buffer;

    MessageSubscriptionList m_subscribed_msg;

    std::shared_ptr< QueueMngr > m_queue_mngr;

    mutable std::mutex m_mutex;

};

} // namespace PubSub

#include "QueueMngr.h"

#endif /* PUBSUB_ENDPOINT_H */
