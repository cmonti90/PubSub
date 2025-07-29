#ifndef PUBSUB_MESSAGE_CONTAINER_H
#define PUBSUB_MESSAGE_CONTAINER_H

namespace Intf
{
template< typename ...Args >
struct MessageContainer : public Args...
{
    void initialize()
    {
        ( Args::initialize(), ... );
    }

    void reset()
    {
        ( Args::reset(), ... );
    }
};
}

#endif /* PUBSUB_MESSAGE_CONTAINER_H */
