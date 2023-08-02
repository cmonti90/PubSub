#ifndef AC0B43C3_818C_4385_BFC0_B5BCF1E2F672
#define AC0B43C3_818C_4385_BFC0_B5BCF1E2F672

namespace PubSub
{
    class Component
    {
    public:
        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void finalize() = 0;
    };
} // namespace PubSub

#endif /* AC0B43C3_818C_4385_BFC0_B5BCF1E2F672 */
