
#include "Component.h"
#include "QueueMngr.h"

#include <iostream>
namespace PubSub
{
    Component::Component(QueueMngr *queue_mngr, const COMPONENT_LABEL str)
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
        std::unique_lock<std::mutex> lock(m_mutex);

        std::cout << "COMPONENT: Component " << Component_Label << " subscribing" << std::endl;

        std::cout << "COMPONENT: Component " << Component_Label << " subscribing to " << msg->getMessageLabel() << std::endl;

        m_subscribed_msg.insert(std::make_pair(msg->getMessageLabel(), msg_type));

        std::cout << "COMPONENT: Component " << Component_Label << " subscribed to " << msg->getMessageLabel() << std::endl;

        lock.unlock();
        m_condition.notify_one();
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
                status = MESSAGE_AVAILABLE;
            }
        }
        else
        {
            msg_label = m_passive_msg_buffer.begin()->first;
            status = MESSAGE_AVAILABLE;
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

        switch (m_subscribed_msg.find(msg->getMessageLabel())->second)
        {
        case ACTIVE:
        {
            *msg = *(m_active_msg_buffer.find(msg->getMessageLabel())->second);

            std::cout << "COMPONENT::receive(): Component " << Component_Label << " received " << msg->getMessageLabel() << std::endl;
            std::cout << "COMPONENT::receive(): Message " << msg->getMessageLabel() << std::endl;

            m_active_msg_buffer.erase(msg->getMessageLabel());
        }
        break;

        case PASSIVE:
        {
            *msg = *(m_passive_msg_buffer.find(msg->getMessageLabel())->second);
            m_passive_msg_buffer.erase(msg->getMessageLabel());
        }
        break;
        }

        lock.unlock();
        m_condition.notify_one();
    }

    void Component::writeToBuffer(Message *msg)
    {
        switch (m_subscribed_msg.find(msg->getMessageLabel())->second)
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

        if (buffer.count(msg->getMessageLabel()))
        {
            *(buffer[msg->getMessageLabel()]) = *msg;
        }
        else
        {
            buffer.insert(std::make_pair(msg->getMessageLabel(), msg));
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