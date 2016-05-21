#ifndef __STREAMCALLER_HPP__
#define __STREAMCALLER_HPP__
#include <utility>
#include "function_traits.hpp"

using namespace std;



template<typename Stream, typename Func>
struct StackStreamCaller
{

};

template<typename Stream, typename R, typename ...Args>
struct StackStreamCaller<Stream, R(Args...)>
{
    constexpr static int return_arity = std::is_void<Args>::value ? 0 : 1;
    using ReturnType = R;
    constexpr static int Arity = sizeof...(Args);

    /** @brief Calls function with arguments extracted from given stream.
    * @param stream stream from which arguments are extracted.
    * @param function Function to call with those values
    * @return Forwards return value from function.
    */
    template<typename Function>
    static ReturnType Call(Stream &stream, const Function& function)
    {
        return CallImpl(stream, function, std::index_sequence_for<Args...>());
    }

protected:

    /** @brief Helper function, gets std::index_sequence<0..Arity> as third argument
    * @tparam s stream from which arguments are extracted.
    * @param f function Function to call with those values
    * @return Forwards return value from function.
    */
    template<std::size_t... Is, typename Function>
    static ReturnType CallImpl(Stream &s, const Function& f, std::index_sequence<Is...>)
    {
        constexpr int arity_dec = Arity - 1;

        //create tuple with types from Args
        std::tuple< std::decay_t<Args>... > args;

        //fills this tuple from stream (since arguments are placed on stack, we are first reading last element)
        //since fold expressions are distant future, and we need to call "s >> std::get< X >(args)" in specific order,
        //we construct a dummy table of zeroes, with our expression evaluated as side effect. Array constructors are guaranteed to be evaluated in order.
        auto fill_args = { 0,
            ((s >> std::get<arity_dec - Is>(args)), 0)...
        };

        //calls Function with arguments from tuple
        return f(std::get<Is>(args)...);
    }
};

template <typename Stream, typename Func>
auto CallerFromStream(Stream& stream, const Func& func)
{
    using Signature = typename mtl::function_traits<Func>::decayed_signature;
    return StackStreamCaller<Stream, Signature>();
}

template <typename Stream, typename Func>
auto CallFromStream(Stream& stream, const Func& func)
{
    using Signature = typename mtl::function_traits<Func>::decayed_signature;
    return StackStreamCaller<Stream, Signature>().Call(stream, func);
}





#endif
