#ifndef TOOL_C_MULTI_CHAN_THREAD_POOL_IMP_H
#define TOOL_C_MULTI_CHAN_THREAD_POOL_IMP_H
/*
 线程池类
 使用方法
 class A
 {
 ...
 void* fun(void*);
 }
 A a;
 mdf::ThreadPool tp;
 tp.Start(10);
 //linux下必须&A::fun，windows下可使用A::fun，A类函数内部直接fun皆可
 //为了移植性，建议传递&A::fun给Bind()
 tp.Accept( mdf::Executor::Bind(&A::fun), &a, (void*)param );
 t.Accept( mdf::Executor::Bind(&A::fun), &a, (void*)param );
 */

#include <list>
#include <vector>
#include <map>
#include <string>

#include "Thread.h"
#include "Lock.h"
#include "Signal.h"
#include <stddef.h>

namespace mdf {
    class MemoryPool;

//线程信息
    typedef struct MULTI_CHAN_THREAD_CONTEXT {
        Thread thread;
        bool bIdle;       //线程处于空闲
        bool bRun;        //运行控制标志
        std::list<Task*>* tasks;   //队列
        Signal* pSignal;    //信号量

    } MULTI_CHAN_THREAD_CONTEXT;
    typedef std::map<ThreadID, MULTI_CHAN_THREAD_CONTEXT*> multiChanThreadMaps;

    class Task;

    class MemoryPool;

    class MultiChanThreadPool {
    public:
        MultiChanThreadPool();

        ~MultiChanThreadPool();

        void SetOnStart(MethodPointer method, void* pObj, void* pParam);

        void SetOnStart(FuntionPointer fun, void* pParam);

        bool Start(int nMinThreadNum);        //启动线程池

        void Stop();        //关闭所有线程
        //接受任务
        //method为声明为void* fun(void*)的成员函数
        void Accept(const std::string& strIndex, MethodPointer method, void* pObj, void* pParam);

        //接受任务
        //fun为声明为void* fun(void*)的函数
        void Accept(std::string& strIndex, FuntionPointer fun, void* pParam);

        unsigned int GetTaskCount(unsigned short usIndex);  //获取当前任务数量

    protected:
        bool CreateThread(unsigned short nNum);    //在线程池中创建n个线程
        MULTI_CHAN_THREAD_CONTEXT* CreateContext();    //创建一个上下文
        void ReleaseContext(MULTI_CHAN_THREAD_CONTEXT* pContext);    //释放一个上下文
        void* RemoteCall ThreadFunc(void* pParam);    //线程函数
        Task* CreateTask();    //创建一个任务
        void ReleaseTask(Task* pTask);    //释放一个任务
        void PushTask(Task* pTask, unsigned int uiIndex);    //将任务放入线程池执行。※注：pTask必须是new出来的对象，alloc都不行
        Task* PullTask(std::list<Task*>* pTasks);    //取出一个任务
        //备用方法，暂无调用需求
        void StopIdle();    //关闭空闲线程(保证最小线程数)

    protected:
        MemoryPool* m_pTaskPool;    //Task对象池
        Mutex m_taskPoolMutex;    //Task对象池线程安全锁
        MemoryPool* m_pContextPool;    //THREAD_CONTEXT对象池
        Mutex m_contextPoolMutex;    //THREAD_CONTEXT对象池线程安全锁

        unsigned short m_nMinThreadNum;    //线程池中必须存在的最小线程数
        unsigned short m_nThreadNum;    //线程池中启动的线程数
        multiChanThreadMaps m_threads;    //线程表
        Mutex m_threadsMutex;    //线程表线程安全锁
        std::vector<std::list<Task*> > m_tasks; //多队列分组任务表
        Mutex m_tasksMutex;    //任务表线程安全锁
        std::vector<Signal> m_sigNewTask;    //新任务信号
        Task m_taskOnStart;    //开始响应
    };

}    //namespace mdf

#endif
