#ifndef AC0B43C3_818C_4385_BFC0_B5BCF1E2F672
#define AC0B43C3_818C_4385_BFC0_B5BCF1E2F672

#include "Message.h"
#include "Endpoint.h"

#include <string>
#include <vector>
#include <unordered_map>

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
        FAIL = 0,
        SUCCESS = 1
    };

    typedef char *COMPONENT_LABEL;
    typedef std::unordered_map<Message_Label, Message_Type> MessageSubscription;
    typedef std::unordered_map<Message_Label, Message *> MessageBuffer;
    class Component
    {
    public:
        friend class QueueMngr;
        Component() = delete;
        Component(const COMPONENT_LABEL str);
        virtual ~Component() = default;

        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void finalize() = 0;

        COMPONENT_LABEL getComponentLabel() const;

        void subscribe(Message *msg, Message_Type msg_type);
        void send(Message *msg);
        void receive(Message *msg);

    private:
        const COMPONENT_LABEL Component_Label;

        MessageBuffer m_active_msg_buffer;
        MessageBuffer m_passive_msg_buffer;

        MessageSubscription m_subscribed_msg;

        void writeToBuffer(Message *msg);
        void writeToBuffer(Message *msg, MessageBuffer &buffer);

        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;
    };
} // namespace PubSub

#endif /* AC0B43C3_818C_4385_BFC0_B5BCF1E2F672 */
