#ifndef STRING_H
#define STRING_H

#include "ShareObject.h"
#include "FastMemoryPool.h"

namespace mdf {

    class String {
        class Data : public ShareObject {
        public:
            char* m_str;

            virtual void Delete();
        };

    public:
        String(void);

        String(const String& right);

        ~String(void);

        static FastMemoryPool ms_pool;

        //如果非NULL,则结点从内存池分配
        String& operator=(const String& right);

        operator char*();

        operator unsigned char*();

        char* c_str();

    private:

        Data* m_data;
    };

}

#endif //STRING_H
