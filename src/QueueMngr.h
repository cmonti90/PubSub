#ifndef D2C30EC4_7B15_4E5E_9288_151C76998994
#define D2C30EC4_7B15_4E5E_9288_151C76998994

#include "Message.h"
#include "Component.h"
#include "Endpoint.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>
#include <list>


namespace PubSub
{
    class Endpoint;

    typedef std::list<Endpoint*> EndpointList;
    typedef std::unordered_map<Message_Name, EndpointList> SubcriberList;

    class QueueMngr
    {
      public:
        QueueMngr() = default;
        ~QueueMngr() = default;

        void subscribe( Endpoint* endpoint, const Message_Name msgName );
        void unsubscribe( Endpoint* endpoint, const Message_Name msgName );
        void push( const Message* value );
        Message* popFront();
        void dispatch();

      private:
        SubcriberList m_subscriberList;

        std::queue<Message*> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_condition;
    };
} // namespace PubSub

#endif /* D2C30EC4_7B15_4E5E_9288_151C76998994 */
