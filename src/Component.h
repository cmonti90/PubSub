#ifndef PUBSUB_COMPONENT_H
#define PUBSUB_COMPONENT_H

#include "Message.h"

#include "Payload.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace PubSub
{
enum Message_Type : unsigned int
{
    ALL,
    ACTIVE,
    PASSIVE
};

enum MessageStatus : unsigned int
{
    FAIL,
    MESSAGE_AVAILABLE
};

typedef std::string                                                     Component_Label;
typedef std::unordered_map< Message_Name, Message_Type >                MessageSubscription;
typedef std::multimap     < Message_Name, Message* >  MessageBuffer;

class QueueMngr;
class Component
{
public:

    Component() = delete;
    Component( std::shared_ptr< QueueMngr >& queue_mngr, const Component_Label str );
    ~Component() = default;

    virtual void initialize() = 0;
    virtual void update()     = 0;
    virtual void finalize()   = 0;

    Component_Label getComponentLabel() const;

protected:
    friend class Endpoint;
    friend class Thread;
    friend class Thread2;

    virtual bool associateEvent() const;

private:
    friend class QueueMngr;

    const Component_Label label;

    std::shared_ptr< QueueMngr > m_queue_mngr;

    Component( const Component& )             = delete;
    Component& operator=( const Component& )  = delete;
};
} // namespace PubSub

#include "QueueMngr.h"

#endif /* PUBSUB_COMPONENT_H */
