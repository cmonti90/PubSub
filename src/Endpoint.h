#ifndef FCFA445A_B890_45ED_9F29_2A8F78C3E4EE
#define FCFA445A_B890_45ED_9F29_2A8F78C3E4EE

#include "Message.h"

namespace PubSub
{
    class Endpoint
    {
    public:

        struct MessageContainer
        {
            Message *msg;
        };

        Endpoint() = default;
        ~Endpoint() = default;

        void initialize(unsigned int primary_msg_depth, unsigned int secondary_msg_depth);
        void send(Message *data);
        void receive(Message *data);

    private:

        Endpoint(const Endpoint &) = delete;
        Endpoint &operator=(const Endpoint &) = delete;

    };
} // namespace PubSub

#endif /* FCFA445A_B890_45ED_9F29_2A8F78C3E4EE */
