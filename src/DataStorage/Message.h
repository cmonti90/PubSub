#ifndef C01A6769_F315_4B96_9050_6141CE2ECA57
#define C01A6769_F315_4B96_9050_6141CE2ECA57

#include <string>

namespace PubSub
{

    typedef std::string Message_Name;
    typedef unsigned int Message_Label;
    class Message
    {
    public:
        Message(const Message_Name str) : m_label(str) {}

        Message(const Message &obj) : m_label(obj.m_label) {}

        Message& operator=(const Message &obj) = default;

        Message_Name getMessageName() { return m_label; };
        const Message_Name getMessageName() const { return m_label; };

        virtual Message_Label getMessageLabel() const = 0;

        virtual ~Message() = default;

        virtual void reset() = 0;

        virtual Message* clone() const = 0;
        virtual void copy(const Message* msg) = 0;

        Message_Name m_label;
    };

} // namespace PubSub

#endif /* C01A6769_F315_4B96_9050_6141CE2ECA57 */
