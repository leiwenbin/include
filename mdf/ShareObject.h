#ifndef SHAREOBJECT_H
#define SHAREOBJECT_H

#include "FixLengthInt.h"

namespace mdf {
    class ShareObject {
    public:
        ShareObject();

        virtual ~ShareObject();

        void AddRef();

        void Release();

    protected:
        virtual void Delete();

    private:
        uint32 m_refCount;
    };
}

#endif //SHAREOBJECT_H
