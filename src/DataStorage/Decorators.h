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

#endif /* B35D9E1C_6524_48EE_B828_72BF7C4889A3 */
