
#include "QueueMngr.h"
namespace PubSub
{

    void QueueMngr::push(const Message *value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_queue.push(value->clone());

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

        while (!m_queue.empty())
        {
            Message *msg = popFront();

            for (unsigned int i{0u}; i < m_subscriberList[msg->getMessageLabelName()].size(); ++i)
            {
                m_subscriberList[msg->getMessageLabelName()][i]->writeToBuffer(msg->clone());
            }

            delete msg;
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void QueueMngr::getSubscriptionList(Component *comp, MessageSubscription &list)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

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
        }

        lock.unlock();
        m_condition.notify_one();
    }

} // namespace PubSub