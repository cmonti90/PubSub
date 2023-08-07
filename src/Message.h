#ifndef C01A6769_F315_4B96_9050_6141CE2ECA57
#define C01A6769_F315_4B96_9050_6141CE2ECA57

#include <string>

namespace PubSub
{

    typedef std::string Message_Label;
    class Message
    {
    public:
        // Message() : m_label("") {}
        Message(const Message_Label str) : m_label(str) {}
        // Message(const Message &obj) = default;

        Message& operator=(const Message &obj) = default;

        // virtual Message_Label getMessageLabel() = 0;
        Message_Label getMessageLabel(){ return m_label; };
        virtual ~Message() = default;

    private:
        Message_Label m_label;
    };

} // namespace PubSub

#endif /* C01A6769_F315_4B96_9050_6141CE2ECA57 */
