#ifndef AC0B43C3_818C_4385_BFC0_B5BCF1E2F672
#define AC0B43C3_818C_4385_BFC0_B5BCF1E2F672

#include "Message.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

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

    class QueueMngr;
    class Component
    {
    public:
        friend class QueueMngr;
        
        Component() = delete;
        Component(const COMPONENT_LABEL str, QueueMngr *queue_mngr);
        virtual ~Component() = default;

        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void finalize() = 0;

        COMPONENT_LABEL getComponentLabel() const;

        void subscribe(Message *msg, Message_Type msg_type);
        MessageStatus peek(Message_Label &msg_label);
        void send(Message *msg);
        void receive(Message *msg);

        bool hasActiveMessage() const;
        
        void giveSubscriptionListToQueueMngr();

    private:
        const COMPONENT_LABEL Component_Label;

        MessageBuffer m_active_msg_buffer;
        MessageBuffer m_passive_msg_buffer;

        MessageSubscription m_subscribed_msg;

        QueueMngr *m_queue_mngr;

        std::mutex m_mutex;
        std::condition_variable m_condition;

        void writeToBuffer(Message *msg);
        void writeToBuffer(Message *msg, MessageBuffer &buffer);

        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;
    };
} // namespace PubSub

#endif /* AC0B43C3_818C_4385_BFC0_B5BCF1E2F672 */
