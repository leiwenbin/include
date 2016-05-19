// SRWLock.h: interface for the SRWLock class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SRWLOCK_H
#define SRWLOCK_H

#include "FixLengthInt.h"
#include "Lock.h"
#include "Signal.h"

/*
 linux32下的进程可拥有的线程数量限制是1024
 实际上32位，用户空间是3G，也就是3072M，3072M除以每个线程栈大小8M得384，
 所以一个进程最多拥有384个线程。
 即使用 ulimit -s 1024 减小默认的栈大小，也无法突破1024个线程的硬限制，除非重新编译 C 库

 windows系统单进程最多创建子线程数量位2000
 */
#define MAX_THREAD_COUNT 2000    //最大线程数，

class SRWLock {
public:
    SRWLock();

    virtual ~SRWLock();

    void Lock();

    void Unlock();

    void ShareLock();

    void ShareUnlock();

protected:
    typedef struct THREAD_HIS {
        mdf::uint64 threadID;
        mdf::uint16 readCount;
        mdf::uint16 writeCount;
    } THREAD_HIS;

    SRWLock::THREAD_HIS* GetThreadData(bool lock);

private:
    THREAD_HIS m_threadData[MAX_THREAD_COUNT];
    mdf::uint32 m_readCount;
    mdf::uint32 m_writeCount;
    mdf::Mutex m_lock;
    mdf::int32 m_waitWriteCount;
    mdf::Signal m_waitWrite;
    mdf::Mutex m_checkLock;
    mdf::uint64 m_ownerThreadId;
    mdf::Signal m_waitRead;
    bool m_bWaitRead;

};

#endif // ifndef(SRWLOCK_H)
