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

template <typename Message>
struct InputPayloadBase : public PayloadBase<Message>, public PayloadBase<Message>::PayloadType
{
    using PayloadType = typename PayloadBase<Message>::PayloadType;

    virtual void updateExternalPayload()
    {
        static_cast< PayloadType& >( *this ) = PayloadBase<Message>::getInternalPayload();
    }
};

template <typename Message>
struct OutputPayloadBase :  public PayloadBase<Message>, public PayloadBase<Message>::PayloadType
{
    using PayloadType = typename PayloadBase<Message>::PayloadType;

    virtual void updateExternalPayload()
    {
        static_cast< PayloadType& >( *this ) = PayloadBase<Message>::getInternalPayload();
    }

    virtual void updateInternalPayload()
    {
        PayloadBase<Message>::overwriteInternalPayload( static_cast< PayloadType& >( *this ) );
    }
};

template <typename Base>
struct Terminator : public Base
{
    static constexpr decPriorityType DECORATOR_PRIORITY = 999999u;
};

template <typename Base, template <typename> typename Decorator>
struct IsValidDecorator
{
    static_assert( Base::DECORATOR_PRIORITY < Decorator<Base>::DECORATOR_PRIORITY, "Decorators must be in ascending priority!" );

    using type = Decorator<Base>;
};

// D3<D2<D1<Base>>>
template <typename Base, template <typename> typename Decorator = Terminator, template <typename> typename ...More>
struct InheritencePayload
{
    using type = typename InheritencePayload< typename IsValidDecorator<Base, Decorator>::type, More...>::type;
};

template <typename Base>
struct InheritencePayload<Base, Terminator>
{
    using type = Base;
};

template <typename Message, template <typename> typename ...Decorators>
struct InputPayload : public InheritencePayload<InputPayloadBase<Message>, Decorators...>::type
{
    virtual void updateExternalPayload()
    {
        InheritencePayload<InputPayloadBase<Message>, Decorators...>::type::updateExternalPayload();
    }
};

template <typename Message, template <typename> typename ...Decorators>
struct OutputPayload :  public InheritencePayload<OutputPayloadBase<Message>, Decorators...>::type
{
    virtual void updateExternalPayload()
    {
        InheritencePayload<OutputPayloadBase<Message>, Decorators...>::type::updateExternalPayload();
    }

    virtual void updateInternalPayload()
    {
        InheritencePayload<OutputPayloadBase<Message>, Decorators...>::type::updateInternalPayload();
    }
};


#endif /* C3D6F63C_4E40_4AAE_8296_E5302C3C9AEC */
