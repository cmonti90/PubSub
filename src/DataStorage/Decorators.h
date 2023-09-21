#ifndef B35D9E1C_6524_48EE_B828_72BF7C4889A3
#define B35D9E1C_6524_48EE_B828_72BF7C4889A3

#include "Payload.h"

template <typename Payload>
struct Persistent : public Payload
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 100u;

    virtual void reset() override
    {
        // Do nothing
    }
};

template <typename Serializer>
struct PayloadSerializer
{
    static_assert( sizeof(Serializer) == 0, "Serializer must implement serialization method" );
};

template <typename Deserializer>
struct PayloadDeserializer
{
    static_assert( sizeof(Deserializer) == 0, "Deserializer must implement deserialization method" );
};

template <typename InputBase>
struct Serialize : public InputBase,
                   public virtual PayloadSerializer<typename InputBase::PayloadType>
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 1u;

    virtual void initialize() override
    {
        InputBase::initialize();
        PayloadSerializer<typename InputBase::PayloadType>::initialize();
    }

    virtual void reset() override
    {
        InputBase::reset();
        PayloadSerializer<typename InputBase::PayloadType>::reset();
    }

    virtual void updateInternalPayload() override
    {
        this->serialize(*this);
        InputBase::updateInternalPayload();
    }
};

template <typename InputBase>
struct Deserialize : public InputBase,
                     public virtual PayloadDeserializer<typename InputBase::PayloadType>
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 1u;

    virtual void initialize() override
    {
        InputBase::initialize();
        PayloadDeserializer<typename InputBase::PayloadType>::initialize();
    }

    virtual void reset() override
    {
        InputBase::reset();
        PayloadDeserializer<typename InputBase::PayloadType>::reset();
    }

    virtual void updateExternalPayload()
    {
        InputBase::updateExternalPayload();
        this->deserialize(*this);
    }
};

#endif /* B35D9E1C_6524_48EE_B828_72BF7C4889A3 */
