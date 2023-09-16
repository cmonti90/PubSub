#ifndef A8BD1AC4_C289_4FC2_B177_23C26612708C
#define A8BD1AC4_C289_4FC2_B177_23C26612708C

namespace Intf
{
    template<typename ...Args>
    struct MessageContainer : public Args...
    {
        void initialize()
        {
            (Args::initialize(), ...);
        }

        void reset()
        {
            (Args::reset(), ...);
        }
    };
}

#endif /* A8BD1AC4_C289_4FC2_B177_23C26612708C */
