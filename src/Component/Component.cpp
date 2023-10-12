
#include "Component.h"
#include "QueueMngr.h"

namespace PubSub
{
    Component::Component(std::shared_ptr<QueueMngr> &queue_mngr, const Component_Label str)
        : label(str),
          m_queue_mngr(queue_mngr)
    {
    }

    Component_Label Component::getComponentLabel() const
    {
        return label;
    }

    void Component::subscribe(const Message *msg, Message_Type msg_type)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_subscribed_msg.insert(std::make_pair(msg->getMessageName(), msg_type));

        lock.unlock();
        m_condition.notify_one();
    }

    MessageStatus Component::peek(Message_Label &msg_label) const
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        MessageStatus status{FAIL};

        if (m_passive_msg_buffer.empty())
        {
            if (m_active_msg_buffer.empty())
            {
                status = FAIL;
            }
            else
            {
                msg_label = m_active_msg_buffer.begin()->second->getMessageLabel();
                status = MESSAGE_AVAILABLE;
            }
        }
        else
        {
            msg_label = m_passive_msg_buffer.begin()->second->getMessageLabel();
            status = MESSAGE_AVAILABLE;
        }

        lock.unlock();
        m_condition.notify_one();

        return status;
    }

    void Component::send(const Message *msg) const
    {
        m_queue_mngr->push(msg);
    }

    void Component::receive(Message *msg)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        switch (m_subscribed_msg.find(msg->getMessageName())->second)
        {
        case ACTIVE:
        {
            msg->copy(m_active_msg_buffer.lower_bound(msg->getMessageName())->second.get());

            m_active_msg_buffer.erase(m_active_msg_buffer.lower_bound(msg->getMessageName()));
        }
        break;

        case PASSIVE:
        {
            msg->copy(m_passive_msg_buffer.lower_bound(msg->getMessageName())->second.get());

            m_passive_msg_buffer.erase(m_passive_msg_buffer.lower_bound(msg->getMessageName()));
        }
        break;
        default:
            break;
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Component::writeToBuffer(Message *msg)
    {
        switch (m_subscribed_msg.find(msg->getMessageName())->second)
        {

        case ACTIVE:
        {
            writeToBuffer(msg, m_active_msg_buffer);
        }
        break;

        case PASSIVE:
        {
            writeToBuffer(msg, m_passive_msg_buffer);
        }
        break;

        default:
            break;
        }
    }

    void Component::writeToBuffer(Message *msg, MessageBuffer &buffer)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        buffer.insert(MessageBuffer::value_type(msg->getMessageName(), std::unique_ptr<Message>(msg->clone())));

        lock.unlock();
        m_condition.notify_one();
    }

    void Component::removeTopMessage()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (!m_passive_msg_buffer.empty())
        {
            m_passive_msg_buffer.erase(m_passive_msg_buffer.begin());
        }
        else if (!m_active_msg_buffer.empty())
        {
            m_active_msg_buffer.erase(m_active_msg_buffer.begin());
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Component::clear()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_active_msg_buffer.clear();
        m_passive_msg_buffer.clear();

        lock.unlock();
        m_condition.notify_one();
    }

    void Component::giveSubscriptionListToQueueMngr()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_queue_mngr->getSubscriptionList(this, m_subscribed_msg);
    }

    bool Component::hasActiveMessage() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        return !m_active_msg_buffer.empty();
    }

} // namespace PubSub