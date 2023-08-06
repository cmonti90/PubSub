#ifndef C01A6769_F315_4B96_9050_6141CE2ECA57
#define C01A6769_F315_4B96_9050_6141CE2ECA57

#include <string>

namespace PubSub
{

    typedef std::string Message_Label;
    class Message
    {
    public:
        static const Message_Label MESSAGE_LABEL;

        virtual void copyData(Message *msg) = 0;
    };

} // namespace PubSub

#endif /* C01A6769_F315_4B96_9050_6141CE2ECA57 */
