#ifndef PUBSUB_MESSAGE_H
#define PUBSUB_MESSAGE_H

#include <string>

namespace PubSub
{

typedef std::string Message_Name;
typedef unsigned int Message_Label;
class Message
{
public:
    Message( const Message_Name str ) : m_label( str ) {}

    Message( const Message& obj ) : m_label( obj.m_label ) {}

    Message& operator=( const Message& obj ) = default;

    Message_Name getMessageName() const { return m_label; };

    virtual Message_Label getMessageLabel() const = 0;

    virtual ~Message() = default;

    virtual void reset() = 0;

    virtual Message* clone() const = 0;
    virtual void copy( const Message* msg ) = 0;

    Message_Name m_label;
};

} // namespace PubSub

#endif // PUBSUB_MESSAGE_H
