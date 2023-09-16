#ifndef MESSAGEPAYLOADTEMPLATE_H
#define MESSAGEPAYLOADTEMPLATE_H

#include "Message.h"

#define MESSAGE_PAYLOAD(MsgName, PayloadType, MsgLbl)                             \
    class MsgName : public PubSub::Message                                        \
    {                                                                             \
    public:                                                                       \
        MsgName() : Message(#MsgName), payload()                                  \
        {                                                                         \
            payload.Default();                                                    \
        }                                                                         \
                                                                                  \
        ~MsgName() {}                                                             \
                                                                                  \
        MsgName(const MsgName &other) : Message(other), payload(other.payload) {} \
                                                                                  \
        MsgName &operator=(const MsgName &other)                                  \
        {                                                                         \
            payload = other.payload;                                              \
            return *this;                                                         \
        }                                                                         \
                                                                                  \
        PayloadType payload;                                                      \
                                                                                  \
        static constexpr PubSub::Message_Label MESSAGE_LABEL = MsgLbl;            \
                                                                                  \
        PubSub::Message_Label getMessageLabel() const override                    \
        {                                                                         \
            return MESSAGE_LABEL;                                                 \
        }                                                                         \
                                                                                  \
        PubSub::Message *clone() const override                                   \
        {                                                                         \
            return new MsgName(*this);                                            \
        }                                                                         \
                                                                                  \
        void copy(const Message *other) override                                  \
                                                                                  \
        {                                                                         \
            const MsgName *pOther = dynamic_cast<const MsgName *>(other);         \
                                                                                  \
            if (pOther != nullptr)                                                \
            {                                                                     \
                payload = pOther->payload;                                        \
            }                                                                     \
        }                                                                         \
                                                                                  \
        void reset()                                                              \
        {                                                                         \
            payload.Default();                                                    \
        }                                                                         \
    };

#endif /* MESSAGEPAYLOADTEMPLATE_H */