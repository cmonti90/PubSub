#ifndef D2C30EC4_7B15_4E5E_9288_151C76998994
#define D2C30EC4_7B15_4E5E_9288_151C76998994

#include "Message.h"
#include "Component.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

namespace PubSub
{
    typedef std::vector<Component *> ComponentList;
    typedef std::unordered_map<Message_Label, ComponentList> SubcriberList;

    class QueueMngr
    {
    public:
        QueueMngr() = default;
        ~QueueMngr() = default;

        void push(Message *value);
        Message *popFront();
        void dispatch();

        void getSubscriptionList(Component* comp, MessageSubscription &list);

    private:
        std::queue<Message *> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_condition;
        SubcriberList m_subscriberList;
    };
} // namespace PubSub

#endif /* D2C30EC4_7B15_4E5E_9288_151C76998994 */
