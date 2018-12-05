#ifndef FAST_MEMORY_POOL_H
#define FAST_MEMORY_POOL_H

#include "FixLengthInt.h"
//最大支持MAX_THREAD_COUNT个线程在FastMemoryPool对象上并发
#define MAX_THREAD_COUNT 5000
#define ToPool mdf::FastMemoryPool::Free

namespace mdf {

    class FastMemoryPool {
        //内存池，链表形式
        typedef struct POOL {
            int freeCount;    //可分配数量
            unsigned char* buffer;    //n个(sizeof(MEMORY)+m_objectSize)的连续空间
            POOL* next;        //下一个内存池
        } POOL;

        //用于分配的内存单位，链表形式
        typedef struct MEMORY {
            POOL* pPool;        //所在内存池
            unsigned char* buffer;    //分配出去的内存
            long isAlloced;    //0未分配，1已分配，其它值异常
            FastMemoryPool* pThis;        //为了Free()方法可以定义为static
            MEMORY* next;        //下一块待分配内存
        } MEMORY;

    public:
        FastMemoryPool(int objectSize, int count);

        ~FastMemoryPool();

        void* Alloc();        //线程安全的分配
        static void Free(void* pObject);        //线程安全的释放，static方法比成员方法调用方式更灵活，可使用类名、对象、对象指针调用

    protected:
        void* AllocMethod();        //非线程安全的分配
        void Expand();        //扩容
        /*
         合并释放
         Free()只是在内存上做个标记，让UniteFree()合并到1个线程中回收，当没有内存可回收时调用Expand()扩容
         */
        void UniteFree();

    private:
        FastMemoryPool() {
        }

        FastMemoryPool(const FastMemoryPool &right) {
        }

        FastMemoryPool &operator=(const FastMemoryPool &right) {
            return *this;
        }

        int m_objectSize;        //每次分配返回内存的大小
        int m_expandCount;        //每次扩容增加的大小为m_objectSize的内存块数量
        MEMORY* m_header;        //待分配内存链表，表头
        POOL* m_poolHeader;        //内存池链表，表头

        //lock free实现
        FastMemoryPool(int objectSize, int count, uint64 ownerThreadId);        //创建一个只让(ID=ownerThreadId)的线程访问的单线程FastMemoryPool对象
        uint64 m_ownerThreadId;        //允许访问本池子的线程ID

        int m_threadCount;        //访问本对象的线程数，最多支持MAX_THREAD_COUNT个线程并发
        FastMemoryPool* m_poolForThread[MAX_THREAD_COUNT];        //m_threadCount个线程，每个线程1个内存池，单线程访问
        uint64 m_poolIndex[MAX_THREAD_COUNT];        //m_threadCount个线程的线程ID，与m_poolForThread下标相同
    };

}

#endif//FAST_MEMORY_POOL_H
