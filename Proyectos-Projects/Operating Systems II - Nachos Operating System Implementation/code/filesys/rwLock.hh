#ifndef RW_LOCK__HH
#define RW_LOCK__HH

class Thread;
class Condition;
class Lock;

class RWLock {
public:
    RWLock(const char *debugName);
    ~RWLock();

    void AcquireReader();
    void AcquireWriter();

    void ReleaseReader();
    void ReleaseWriter();

    Lock* lck;
private:
    const char *name;

    Condition* cnd;

    Thread* currWriter = nullptr;

    int writersWaiting = 0;
    int activeReaders = 0;

    int writeDepth = 0;
};

#endif // RW_LOCK__HH