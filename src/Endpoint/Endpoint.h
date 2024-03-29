#ifndef CC22A0C1_50F1_47C3_AA9D_7A844A3CF145
#define CC22A0C1_50F1_47C3_AA9D_7A844A3CF145

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
        Endpoint( std::shared_ptr<QueueMngr>& queue_mngr );
        virtual ~Endpoint();

        void configure( std::shared_ptr<QueueMngr>& queue_mngr );

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

      private:
        friend class QueueMngr;

        void writeToBuffer( Message* msg );
        void writeToBuffer( Message* msg, MessageBuffer& buffer );

        unsigned int m_active_depth;
        unsigned int m_passive_depth;

        MessageBuffer m_active_msg_buffer;
        MessageBuffer m_passive_msg_buffer;

        MessageSubscriptionList m_subscribed_msg;

        std::shared_ptr< QueueMngr > m_queue_mngr;

        mutable std::mutex m_mutex;
        mutable std::condition_variable m_condition;

    };

} // namespace PubSub

#include "QueueMngr.h"

#endif /* CC22A0C1_50F1_47C3_AA9D_7A844A3CF145 */
