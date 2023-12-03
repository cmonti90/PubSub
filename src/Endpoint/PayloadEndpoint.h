#ifndef C29A0A47_6FA6_44FD_A165_4D0410005BA8
#define C29A0A47_6FA6_44FD_A165_4D0410005BA8

#include "Endpoint.h"
#include "Payload.h"

namespace PubSub
{
    class PayloadEndpoint : public Endpoint
    {
      public:
        PayloadEndpoint() = delete;

        PayloadEndpoint( std::shared_ptr<QueueMngr>& queue_mngr )
            : Endpoint( queue_mngr )
        {
        }

        virtual ~PayloadEndpoint() = default;

        template <typename Msg, typename Container>
        void subscribe( const Container& data, const Message_Type msg_type = Message_Type::ACTIVE )
        {
            Endpoint::subscribe( &static_cast< const InputPayloadBase<Msg>&>( data ).getInternalMsg(), msg_type );
        }

        template <typename Msg, typename Container>
        void unsubscribe( const Container* data )
        {
            Endpoint::unsubscribe( &static_cast< const InputPayloadBase<Msg>& >( data ).getInternalMsg() );
        }

        template < typename Msg, typename Container >
        void send( Container& data ) const
        {
            static_cast< OutputPayloadBase< Msg >& >( data ).updateInternalPayload();
            Endpoint::send( &static_cast< OutputPayloadBase< Msg >& >( data ).getInternalMsg() );
        }

        template < typename Msg, typename Container >
        void receive( Container& msg )
        {
            Endpoint::receive( &static_cast< InputPayloadBase< Msg >& >( msg ).getInternalMsg() );
            static_cast< InputPayloadBase< Msg >& >( msg ).updateExternalPayload();
        }

    };
    
} // namespace PubSub

#endif /* C29A0A47_6FA6_44FD_A165_4D0410005BA8 */
