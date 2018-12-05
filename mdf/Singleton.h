#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace mdf {

    template<class T>
    class Singleton {
    public:
        static T* Instance() {
            static T tValue;
            return &tValue;
        }

    protected:
        Singleton() {
        }

        virtual ~Singleton() {
        }

    private:
        Singleton& operator=(const Singleton&) {
            return Singleton;
        }

        Singleton(const Singleton&) {
        }
    };

}
// namespace mdf

#endif
