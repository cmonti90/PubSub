
#include "Component.h"
#include "QueueMngr.h"

namespace PubSub
{
    Component::Component(const COMPONENT_LABEL str, QueueMngr *queue_mngr)
        : Component_Label(str),
          m_queue_mngr(queue_mngr)
    {
    }

    COMPONENT_LABEL Component::getComponentLabel() const
    {
        return Component_Label;
    }

    void Component::subscribe(Message *msg, Message_Type msg_type)
    {
        m_subscribed_msg.insert(std::make_pair(msg->MESSAGE_LABEL, msg_type));
    }

    MessageStatus Component::peek(Message_Label &msg_label)
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
                msg_label = m_active_msg_buffer.begin()->first;
                status = SUCCESS;
            }
        }
        else
        {
            msg_label = m_passive_msg_buffer.begin()->first;
            status = SUCCESS;
        }

        lock.unlock();
        m_condition.notify_one();

        return status;
    }

    void Component::send(Message *msg)
    {
        m_queue_mngr->push(msg);
    }

    void Component::receive(Message *msg)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        switch (m_subscribed_msg.find(msg->MESSAGE_LABEL)->second)
        {
        case ACTIVE:
        {
            *msg = *(m_active_msg_buffer.find(msg->MESSAGE_LABEL)->second);
            m_active_msg_buffer.erase(msg->MESSAGE_LABEL);
        }
        break;

        case PASSIVE:
        {
            *msg = *(m_passive_msg_buffer.find(msg->MESSAGE_LABEL)->second);
            m_passive_msg_buffer.erase(msg->MESSAGE_LABEL);
        }
        break;
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Component::writeToBuffer(Message *msg)
    {
        switch (m_subscribed_msg.find(msg->MESSAGE_LABEL)->second)
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
        {
            writeToBuffer(msg, m_passive_msg_buffer);
        }
        }
    }

    void Component::writeToBuffer(Message *msg, MessageBuffer &buffer)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (buffer.count(msg->MESSAGE_LABEL))
        {
            *(buffer[msg->MESSAGE_LABEL]) = *msg;
        }
        else
        {
            buffer.insert(std::make_pair(msg->MESSAGE_LABEL, msg));
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Component::giveSubscriptionListToQueueMngr()
    {
        m_queue_mngr->getSubscriptionList(this, m_subscribed_msg);
    }

    bool Component::hasActiveMessage() const
    {
        return !m_active_msg_buffer.empty();
    }

} // namespace PubSub