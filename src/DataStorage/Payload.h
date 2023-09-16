#ifndef C3D6F63C_4E40_4AAE_8296_E5302C3C9AEC
#define C3D6F63C_4E40_4AAE_8296_E5302C3C9AEC

#include "Message.h"

typedef unsigned int decPriorityType;

template <typename Message>
struct PayloadBase
{

private:
    Message message_;

public:
    static constexpr decPriorityType DECORATOR_PRIORITY = 0u;

    using PayloadType = decltype( message_.payload );

    PayloadBase() : message_(Message()) {}
    PayloadBase( Message& msg ) : message_( msg ) {}

    virtual ~PayloadBase()
    {
    }

    void overwriteInternalPayload( const PayloadType& payload )
    {
        message_.payload = payload;
        updateExternalPayload();
    }

    void copyFromPayload( const PayloadType& payload )
    {
        overwriteInternalPayload( payload );
        updateExternalPayload();
    }

    void initialize()
    {
        message_ = Message();
        updateExternalPayload();
    }

    virtual void reset()
    {
        message_ = Message();
        updateExternalPayload();
    }

    PubSub::Message* getInternalMsg()
    {
        return &message_;
    }

    const PubSub::Message* getInternalMsg() const
    {
        return &message_;
    }

// protected:

    PayloadType getInternalPayload() const
    {
        return message_.payload;
    }

    virtual void updateExternalPayload() = 0;

};

struct Terminator
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 999999u;
};

template <typename Message, typename ...Decorators>
struct InheritencePayload : public PayloadBase<Message>, public PayloadBase<Message>::PayloadType, public Decorators...
{
    using PayloadType = typename PayloadBase<Message>::PayloadType;
};

template <typename Message, typename ...Decorators>
struct InputPayload : public InheritencePayload<Message, Decorators...>
{
    using PayloadType = typename InheritencePayload<Message, Decorators...>::PayloadType;

    virtual void updateExternalPayload()
    {
        static_cast< PayloadType& >( *this ) = PayloadBase<Message>::getInternalPayload();
    }
};

template <typename Message, typename ...Decorators>
struct OutputPayload :  public InheritencePayload<Message, Decorators...>
{
    using PayloadType = typename InheritencePayload<Message, Decorators...>::PayloadType;

    virtual void updateExternalPayload()
    {
        static_cast< PayloadType& >( *this ) = PayloadBase<Message>::getInternalPayload();
    }

    virtual void updateInternalPayload()
    {
        PayloadBase<Message>::overwriteInternalPayload( static_cast< PayloadType& >( *this ) );
    }

};


#endif /* C3D6F63C_4E40_4AAE_8296_E5302C3C9AEC */
