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
	constexpr static int return_arity = std::is_void<R>::value ? 0 : 1;
	using ReturnType = R;
	//using ReturnType = std::conditional_t<return_arity == 0, nothing_t, R>;

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
	static auto CallImpl(Stream &s, const Function& f, std::index_sequence<Is...>)
	{
		constexpr int arity_dec = Arity - 1;

		//create tuple with types from Args
		std::tuple< std::decay_t<Args>... > args;

		//copy arguments from lua stack to tuple
		((s >> std::get<arity_dec - Is>(args)), ...);

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


template <typename Stream, typename Func>
int CallFromStreamResultToStream(Stream& stream, const Func& func)
{
	using traits = mtl::function_traits<Func>;
	using Signature = typename traits::decayed_signature;
	if constexpr (std::is_void_v<traits::return_type>)
	{
		StackStreamCaller<Stream, Signature>().Call(stream, func);
		return 0;
	}
	else
	{
		stream << StackStreamCaller<Stream, Signature>().Call(stream, func);
		return 1;
	}
}


#endif
