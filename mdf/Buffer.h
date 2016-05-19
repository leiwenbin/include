#ifndef BUFFER_H
#define BUFFER_H

#include "ShareObject.h"
#include "FastMemoryPool.h"
#include "Lock.h"

namespace mdf {
/*
 缓冲类型
 */
#define EXPAND_SIZE 8192    //每次扩容大小

    class Buffer : public ShareObject {
    public:
        Buffer();

        Buffer(bool usePool);

        virtual ~Buffer();

        void AddData(unsigned char* pData, int32 size);    //增加数据
        bool GetData(unsigned char* pData, int32 size, bool checkData = false);    //取数据
        bool Seek(int32 size);    //跳过（删除）size byte长度的数据
        int32 Size();

        void Clear();

    private:
        static FastMemoryPool ms_pool;    //如果非NULL,则结点从内存池分配
        typedef struct LIST_NODE {
            unsigned char pData[EXPAND_SIZE];    //数据
            int32 size;                    //数据长度
            int32 pos;                    //已处理到数据的位置
            LIST_NODE* pNext;            //下一个数据
        } LIST_NODE;

        LIST_NODE* m_pHeader;
        LIST_NODE* m_pTail;

        int32 m_sumSize;            //链表所有结点数据总长度
        bool m_usePool;            //使用内存池创建缓冲
        mdf::Mutex m_lock;            //线程安全锁
    };

}

#endif //BUFFER_H
