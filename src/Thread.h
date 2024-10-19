#ifndef PUBSUB_THREAD_H
#define PUBSUB_THREAD_H


#include "Component.h"
#include <string>
#include <vector>
#include <mutex>

namespace PubSub
{

typedef std::string ThreadName;
typedef std::vector< Component* > ComponentList;


class Thread
{
public:

    Thread() = delete;
    Thread( const ThreadName& name );
    ~Thread();

    Thread( Thread& obj );

protected:

    friend class Module;

    enum ThreadState
    {
        INITIALIZE,
        UPDATE,
        FINALIZE
    };

    virtual void Run( const ThreadState& state );

    void AddComp( Component* comp );

    ThreadName GetThreadName() const;

    void RunInitialize();
    void RunUpdate();
    void RunFinalize();

    mutable std::mutex mtx;

    ComponentList m_procs;

private:

    const ThreadName m_threadName;

    Thread( const Thread& )             = delete;
    Thread& operator=( const Thread& )  = delete;
};

} // namespace PubSub

#endif /* PUBSUB_THREAD_H */
