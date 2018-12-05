// Signal.h: interface for the Signal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDF_SINGLE_H
#define MDF_SINGLE_H

/*
 信号类
 效果
 如果没有线程在等待
 多次Notify，只会保留第1次有效，之后全部丢失
 */
#ifdef WIN32
//为了不include <windows.h>
typedef void *HANDLE;
#else

#include <pthread.h>
#include <semaphore.h>

#endif

namespace mdf {

    class Signal {
    public:
        Signal();

        virtual ~Signal();

        bool Wait(long nMillSecond = -1);

        bool Notify();

    private:
#ifdef WIN32
        HANDLE m_signal;
#else
        int m_waitCount;
        sem_t m_signal;
#endif
    };

}//namespace mdf

#endif // MDF_SINGLE_H
