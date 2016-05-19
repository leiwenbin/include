// Timer.h: interface for the Timer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef TIMER_H
#define TIMER_H

#include "FixLengthInt.h"
#include "Thread.h"
#include "Lock.h"
#include "Signal.h"
#include "mapi.h"

#include <map>

namespace mdf {
    class ShareObject;

    class Timer {
    public:
        Timer();

        virtual ~Timer();

        /*
         下派生类指针一旦转换成了void*，就无法转换回正确的基类指针地址
         所以必须要有ShareObject参数接收对象基类指针，操作基类成员

         pObj是一个ShareObject的派生类，以确保pObj指向的对象不会在定时器退出前被释放
         */
        void SetTimer(int eventId, uint64 second, MethodPointer method, void* pObj, ShareObject* pSafeObj = NULL, void* pData = NULL, int dataSize = 0, int repeat = -1);

        void SetTimer(int eventId, uint64 second, FuntionPointer fun, void* pData = NULL, int dataSize = 0, int repeat = -1);

        void KillTimer(int eventId);

        void KillAllTimer();

        void* RemoteCall Work(void* param);

    private:
        Thread m_worker;

        typedef struct TASK {
            int eventId;
            Task worker;
            uint64 millSecond;
            uint64 lastTime;
            int repeat;
            int state;
            Mutex lock;

            void Release();

            int ref;
            ShareObject* pObj;
            void* pData;
            int dataSize;
        } TASK;
        std::map<int, TASK*> m_waitTasks;
        Mutex m_lock;
        Signal m_sigRun;
        uint64 m_threadId;
    };

}

#endif // !defined TIMER_H
