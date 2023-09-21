#ifndef AC0B43C3_818C_4385_BFC0_B5BCF1E2F672
#define AC0B43C3_818C_4385_BFC0_B5BCF1E2F672

#include "Message.h"

#include "Payload.h"

#include <string>
#include <vector>
#include <unordered_map>
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
        FAIL = 0,
        MESSAGE_AVAILABLE = 1
    };

    typedef std::string Component_Label;
    typedef std::unordered_map<Message_Name, Message_Type> MessageSubscription;
    typedef std::unordered_map<Message_Name, std::unique_ptr<Message>> MessageBuffer;

    class QueueMngr;
    class Component
    {
    public:
        friend class QueueMngr;

        Component() = delete;
        Component( std::shared_ptr<QueueMngr>& queue_mngr, const Component_Label str );
        virtual ~Component() = default;

        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void finalize() = 0;

        Component_Label getComponentLabel() const;

        void subscribe( const Message* msg, Message_Type msg_type = ACTIVE );

        template <typename Msg, typename Container>
        void subscribe( const Container& data, Message_Type msg_type = ACTIVE )
        {
            subscribe( &static_cast< const InputPayloadBase<Msg>&>( data ).getInternalMsg(), msg_type );
        }

        MessageStatus peek( Message_Label& msg_label );

        void send( const Message* msg );

        template <typename Msg, typename Container>
        void send( Container& data )
        {
            static_cast< OutputPayloadBase<Msg>&>( data ).updateInternalPayload();
            send( &static_cast< OutputPayloadBase<Msg>&>( data ).getInternalMsg() );
        }

        void receive( Message* msg );

        template <typename Msg, typename Container>
        void receive( Container& data )
        {
            receive( &static_cast< PayloadBase<Msg>&>( data ).getInternalMsg() );
            static_cast< InputPayloadBase<Msg>&>( data ).updateExternalPayload();
        }

        void removeTopMessage();
        void clear();

        bool hasActiveMessage() const;

        void giveSubscriptionListToQueueMngr();

    private:
        const Component_Label label;

        MessageBuffer m_active_msg_buffer;
        MessageBuffer m_passive_msg_buffer;

        MessageSubscription m_subscribed_msg;

        std::shared_ptr<QueueMngr> m_queue_mngr;

        std::mutex m_mutex;
        std::condition_variable m_condition;

        void writeToBuffer( Message* msg );
        void writeToBuffer( Message* msg, MessageBuffer& buffer );

        Component( const Component& ) = delete;
        Component& operator=( const Component& ) = delete;
    };
} // namespace PubSub

#include "QueueMngr.h"

#endif /* AC0B43C3_818C_4385_BFC0_B5BCF1E2F672 */
