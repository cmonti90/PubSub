
#include "QueueMngr.h"

namespace PubSub
{

    void QueueMngr::push(Message* value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(value);
        lock.unlock();
        m_condition.notify_one();
    }

    Message* QueueMngr::popFront()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_queue.empty())
        {
            m_condition.wait(lock);
        }

        Message* value = m_queue.front();
        m_queue.pop();
        return value;
    }

    void QueueMngr::dispatch()
    {
        Message* msg = popFront();

        for (unsigned int i{0u}; i < m_subscriberList[msg->MESSAGE_LABEL].size(); ++i)
        {
             m_subscriberList[msg->MESSAGE_LABEL][i]->writeToBuffer(msg);
        }
    }


} // namespace PubSub