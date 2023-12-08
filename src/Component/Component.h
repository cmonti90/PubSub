#ifndef AC0B43C3_818C_4385_BFC0_B5BCF1E2F672
#define AC0B43C3_818C_4385_BFC0_B5BCF1E2F672

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
    typedef std::multimap     < Message_Name, std::unique_ptr< Message > >  MessageBuffer;

    class QueueMngr;
    class Component
    {
      public:

        Component() = delete;
        Component( std::shared_ptr<QueueMngr>& queue_mngr, const Component_Label str );
        virtual ~Component() = default;

        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void finalize() = 0;

        Component_Label getComponentLabel() const;

      protected:
        friend class Endpoint;
        friend class Thread;

        virtual bool associateEvent() const;

      private:
        friend class QueueMngr;

        const Component_Label label;

        std::shared_ptr< QueueMngr > m_queue_mngr;

        Component( const Component& ) = delete;
        Component& operator=( const Component& ) = delete;
    };
} // namespace PubSub

#include "QueueMngr.h"

#endif /* AC0B43C3_818C_4385_BFC0_B5BCF1E2F672 */
