#ifndef __PROXY_HPP__
#define __PROXY_HPP__
#include <mutex>
template<class T, class MutexInjection = std::mutex>
class ptr_holder
{
public:
    ptr_holder(T* ptr): ptr_(ptr) {}
    class proxy : private std::lock_guard<MutexInjection>
    {
    public:
        proxy(T* ptr, MutexInjection& mutex) : std::lock_guard<MutexInjection>(mutex), ptr_(ptr) {}
        T* operator->() const
        {
            return ptr_;
        }
    private:
        T* ptr_;
    };
    proxy operator->() const
    {
        return proxy(ptr_, mutex_);
    }
    //}
private:
    T* ptr_;
    mutable MutexInjection mutex_;
};
#endif // __PROXY_HPP__