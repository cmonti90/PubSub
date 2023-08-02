#ifndef C01A6769_F315_4B96_9050_6141CE2ECA57
#define C01A6769_F315_4B96_9050_6141CE2ECA57

#include <string>

namespace PubSub
{
    class Message
    {
        virtual void write(void *input) = 0;
        virtual void copy(void *output) = 0;

        typedef std::string Message_Label;

        static const Message_Label MESSAGE_LABEL;
    };

} // namespace PubSub

#endif /* C01A6769_F315_4B96_9050_6141CE2ECA57 */
