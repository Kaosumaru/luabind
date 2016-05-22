#ifndef MTL_MEM_FN_SHARED_PTR
#define MTL_MEM_FN_SHARED_PTR

#include <tuple>

namespace mtl
{
    namespace impl
    {
        template<typename T>
        struct mem_fn_shared_ptr
        {

        };

        // member function pointer
        template<class C, class R, class... Args>
        struct mem_fn_shared_ptr<R(C::*)(Args...)>
        {
            using member_type = R(C::*)(Args...);
            static auto transform(member_type member)
            {
                return [=](const std::shared_ptr<C>& s, Args... args)
                {
                    auto &self = *s;
                    return (self.*member)(std::forward<Args>(args)...);
                };
            }
        };
    }


    template<typename T>
    auto mem_fn_shared_ptr(T t)
    {
        return impl::mem_fn_shared_ptr<T>::transform(t);
    };

}

#endif
