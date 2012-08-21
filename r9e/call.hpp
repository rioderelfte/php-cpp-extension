// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_CALL_HPP_INCLUDED
#define R9E_CALL_HPP_INCLUDED

#include <tuple>

namespace r9e {
namespace internal {

template<int... I>
    struct Sequence {};

template <int N, int... I>
    struct GenerateSequence
    : public GenerateSequence<N - 1, N, I...>
{};

template <int... I>
    struct GenerateSequence<0, I...>
{
    typedef Sequence<0, I...> type;
};

template<
    int... I,
    typename F,
    typename T
>
    auto call(
        F func,
        T &&params,
        Sequence<I...>
    ) -> decltype(
        func(
            std::forward<
                decltype(std::get<I>(std::forward<T>(params)))
            >(
                std::get<I>(std::forward<T>(params))
            )...
        )
    )
{
    return func(
        std::forward<
            decltype(std::get<I>(std::forward<T>(params)))
        >(
            std::get<I>(std::forward<T>(params))
        )...
    );
}

template <
    typename T,
    int... I,
    typename P
>
    T construct(
        P &&params,
        Sequence<I...>
    ) {
    return T{
        std::forward<
            decltype(std::get<I>(std::forward<P>(params)))
        >(
            std::get<I>(std::forward<P>(params))
        )...
    };
}

}

template<
    typename F,
    typename T
>
    auto call(
        F func,
        T &&params
    ) -> decltype(
        internal::call(
            func,
            std::forward<T>(params),
            typename internal::GenerateSequence<
                std::tuple_size<typename std::remove_reference<T>::type>::value - 1
            >::type()
        )
    )
{
    return internal::call(
        func,
        std::forward<T>(params),
        typename internal::GenerateSequence<
            std::tuple_size<typename std::remove_reference<T>::type>::value - 1
        >::type()
    );
}

template <
    typename T,
    typename P
>
    T construct(
        P &&params
    ) {
    return internal::construct<T>(
        std::forward<P>(params),
        typename internal::GenerateSequence<
            std::tuple_size<typename std::remove_reference<P>::type>::value - 1
        >::type()
    );
}

}

#endif /* end of include guard */
