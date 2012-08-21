// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_VARIADICALGORITHMS_HPP_INCLUDED
#define R9E_VARIADICALGORITHMS_HPP_INCLUDED

namespace r9e {

template <typename... Ts>
    void doNothing(Ts&&...)
{}

class DoNothing
{
public:
    DoNothing()
    {}

    template <class... Ts>
        DoNothing(const Ts&...)
    {}
};

bool all() {
    return true;
}

template <typename T, typename... Ts>
    bool all(const T &t, const Ts&... ts) {
    return static_cast<bool>(t) && all(ts...);
}

bool any() {
    return false;
}

template <typename T, typename... Ts>
    bool any(const T &t, const Ts&... ts) {
    return static_cast<bool>(t) || any(ts...);
}

}

#endif /* end of include guard */
