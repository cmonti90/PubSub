
#include "QueueMngr.h"
#include <iostream>
namespace PubSub
{

    void QueueMngr::push(Message *value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_queue.push(value->clone());

        std::cout << "QUEUEMNGR: Pushing message " << value->getMessageLabel() << " with data = " << value->dataStructure() << std::endl;

        lock.unlock();
        m_condition.notify_one();
    }

    Message *QueueMngr::popFront()
    {

        Message *value = m_queue.front();
        m_queue.pop();

        return value;
    }

    void QueueMngr::dispatch()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        std::cout << "QUEUEMNGR: Dispatching messages" << std::endl;

        while (!m_queue.empty())
        {
            std::cout << "QUEUEMNGR: Queue size " << m_queue.size() << std::endl;
            std::cout << "QUEUEMNGR: popping message" << std::endl;
            Message *msg = popFront();
            std::cout << "QUEUEMNGR: popped message" << std::endl;

            std::cout << "QUEUEMNGR: Dispatching message " << msg->getMessageLabel() << " with data = " << msg->dataStructure();

            for (unsigned int i{0u}; i < m_subscriberList[msg->getMessageLabel()].size(); ++i)
            {
                std::cout << " to " << m_subscriberList[msg->getMessageLabel()][i]->getComponentLabel() << std::endl;
                m_subscriberList[msg->getMessageLabel()][i]->writeToBuffer(msg->clone());
            }

            delete msg;
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void QueueMngr::getSubscriptionList(Component *comp, MessageSubscription &list)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        std::cout << "QUEUEMNGR: Getting subscription list for " << comp->getComponentLabel() << std::endl;

        for (auto it = list.begin(); it != list.end(); ++it)
        {
            if (m_subscriberList.count(it->first))
            {
                m_subscriberList.find(it->first)->second.push_back(comp);
            }
            else
            {
                m_subscriberList.insert(std::make_pair(it->first, std::vector<Component *>{comp}));
            }

            std::cout << "QUEUEMNGR: Component " << comp->getComponentLabel() << " subscribed to " << it->first << std::endl;
        }

        lock.unlock();
        m_condition.notify_one();
    }

} // namespace PubSub