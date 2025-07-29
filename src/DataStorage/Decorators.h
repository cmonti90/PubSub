#ifndef PUBSUB_DECORATORS_H
#define PUBSUB_DECORATORS_H

#include "Payload.h"

template< typename Payload >
struct Persistent : public Payload
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 100u;

    virtual void reset() override
    {
        // Do nothing
    }
};

template< typename Serializer >
struct PayloadSerializer
{
    static_assert( sizeof( Serializer ) == 0, "Serializer must implement serialization method" );
};

template< typename Deserializer >
struct PayloadDeserializer
{
    static_assert( sizeof( Deserializer ) == 0, "Deserializer must implement deserialization method" );
};

template< typename OutputBase >
struct Serialize :
    public OutputBase,
    public virtual PayloadSerializer< typename OutputBase::PayloadType >
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 1u;

    static_assert( std::is_same< typename OutputBase::tagType, OutputTag >::value, "Serialize decorator must be used with output type, i.e. OutputPayload" );

    virtual void initialize() override
    {
        OutputBase::initialize();
        PayloadSerializer< typename OutputBase::PayloadType >::initialize();
    }

    virtual void reset() override
    {
        OutputBase::reset();
        PayloadSerializer< typename OutputBase::PayloadType >::reset();
    }

    virtual void updateInternalPayload() override
    {
        this->serialize( *this );
        OutputBase::updateInternalPayload();
    }
};

template< typename InputBase >
struct Deserialize :
    public InputBase,
    public virtual PayloadDeserializer< typename InputBase::PayloadType >
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 1u;

    static_assert( std::is_same< typename InputBase::tagType, InputTag >::value, "Deserialize decorator must be used with input type, i.e. InputPayload" );

    virtual void initialize() override
    {
        InputBase::initialize();
        PayloadDeserializer< typename InputBase::PayloadType >::initialize();
    }

    virtual void reset() override
    {
        InputBase::reset();
        PayloadDeserializer< typename InputBase::PayloadType >::reset();
    }

    virtual void updateExternalPayload()
    {
        InputBase::updateExternalPayload();
        this->deserialize( *this );
    }
};

#endif /* PUBSUB_DECORATORS_H */
