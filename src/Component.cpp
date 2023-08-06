
#include "Component.h"

namespace PubSub
{
    Component::Component(const COMPONENT_LABEL str)
        : Component_Label(str)
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

    void Component::send(Message *msg)
    {
    }

    void Component::receive(Message *msg)
    {
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
        if (buffer.count(msg->MESSAGE_LABEL))
        {
            *(buffer[msg->MESSAGE_LABEL]) = *msg;
        }
        else
        {
            buffer.insert(std::make_pair(msg->MESSAGE_LABEL, msg));
        }
    }
} // namespace PubSub