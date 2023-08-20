#ifndef C3D6F63C_4E40_4AAE_8296_E5302C3C9AEC
#define C3D6F63C_4E40_4AAE_8296_E5302C3C9AEC

template <typename T>
struct PayloadBase
{

private:
    T message_;
public:
    using type = decltype( message_.payload );

    PayloadBase() : message_(T()) {}
    PayloadBase( T& data ) : message_( data ) {}

    virtual ~PayloadBase()
    {
    }

    virtual void updateExternalPayload() = 0;

    void overwriteInternalPayload( const type& payload )
    {
        message_.payload = payload;
        updateExternalPayload();
    }

    void copyFromPayload( const type& data )
    {
        overwriteInternalPayload( data );
        updateExternalPayload();
    }

    void reset()
    {
        message_ = T();
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

    type getInternalPayload() const
    {
        return message_.payload;
    }

};

template <typename T>
struct InputPayload : PayloadBase<T>,
    PayloadBase<T>::type
{
    using type = typename PayloadBase<T>::type;

    InputPayload() : PayloadBase<T>() {}
    InputPayload( T& data ) : PayloadBase<T>( data ) {}
    ~InputPayload() {}

    virtual void updateExternalPayload() override
    {
        static_cast< type& >( *this ) = PayloadBase<T>::getInternalPayload();
    }
};

template <typename T>
struct OutputPayload :  PayloadBase<T>,
    PayloadBase<T>::type
{
    using type = typename PayloadBase<T>::type;

    OutputPayload() : PayloadBase<T>() {}
    OutputPayload( T& data ) : PayloadBase<T>( data ) {}
    ~OutputPayload() {}

    virtual void updateExternalPayload() override
    {
        static_cast< type& >( *this ) = PayloadBase<T>::getInternalPayload();
    }

    virtual void updateInternalPayload()
    {
        PayloadBase<T>::overwriteInternalPayload( static_cast< type& >( *this ) );
    }

};


#endif /* C3D6F63C_4E40_4AAE_8296_E5302C3C9AEC */
