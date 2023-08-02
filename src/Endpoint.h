#ifndef FCFA445A_B890_45ED_9F29_2A8F78C3E4EE
#define FCFA445A_B890_45ED_9F29_2A8F78C3E4EE

namespace PubSub
{
    class Endpoint
    {
    public:
        enum Message_Type : unsigned int
        {
            ALL,
            PRIMARY,
            SECONDARY
        };

        void subscribe( void* container, Message_Type type );
        void initialize(unsigned int primary_msg_depth, unsigned int secondary_msg_depth);
        void send(void *data);
        void receive(void *data);
    };
} // namespace PubSub

#endif /* FCFA445A_B890_45ED_9F29_2A8F78C3E4EE */
