// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_PHP_PARAMETERS_HPP_INCLUDED
#define R9E_PHP_PARAMETERS_HPP_INCLUDED

#include <functional>

#include <php.h>

#include <r9e/call.hpp>
#include <r9e/variadicAlgorithms.hpp>
#include <r9e/php/dataTypes.hpp>

namespace r9e {
namespace php {
namespace internal {

// Parameterdatentypen:
// bool
// long
// double
// r9e::php::String
// std::string
// r9e::php::Resource
// r9e::php::Array
// r9e::php::Object
// r9e::php::ZValue

template <typename T>
    class Parameter;

template<>
    class Parameter<bool>
{
public:
    typedef bool ParameterType;

    static const char parameterCode = 'b';

    Parameter(bool &b)
    : b_(&b)
    {}

    bool commit()
    {
        *b_ = zb_;
        return true;
    }

    std::tuple<zend_bool*> getParameters()
    {
        return std::make_tuple(&zb_);
    }

private:
    bool *b_;
    zend_bool zb_ = 0;
};

template<>
    class Parameter<String>
{
public:
    typedef const String& ParameterType;

    static const char parameterCode = 's';

    Parameter(String &str)
    : str_(str)
    , data_(0)
    , length_(0)
    {}

    bool commit() {
        str_ = String(data_, length_, false);
        return true;
    }

    std::tuple<char**, int*> getParameters()
    {
        return std::make_tuple(&data_, &length_);
    }

private:
    String &str_;
    char *data_;
    int length_;
};

template <>
    class Parameter<std::string>
{
public:
    typedef const std::string& ParameterType;

    static const char parameterCode = 's';

    Parameter(std::string &str)
    : str_(str)
    {}

    bool commit() {
        str_.assign(data_, length_);
        return true;
    }

    std::tuple<char**, int*> getParameters()
    {
        return std::make_tuple(&data_, &length_);
    }

private:
    std::string &str_;
    char *data_;
    int length_;
};

template <typename... Ts>
    bool parseArgumentsHelper(
        int ht TSRMLS_DC,
        const char *paramCodes,
        Ts&... vs
    )
{
    auto paramTuple = std::tuple_cat(
        std::make_tuple(ht TSRMLS_CC, paramCodes),
        vs.getParameters()...
    );

    return call(zend_parse_parameters, paramTuple) != FAILURE;
}

template <typename... Ts>
    void parseArgumentsCommiter(
        int ht TSRMLS_DC,
        const char *paramCodes,
        Ts&... vs
    )
{
    doNothing(
        vs.commit()...
    );
}

}

template <typename... Ts>
    bool parseArguments(
        int ht TSRMLS_DC,
        Ts&... vs
    )
{
    char paramCodes[] {
        internal::Parameter<Ts>::parameterCode...,
        0
    };

    auto params = std::make_tuple(
        ht TSRMLS_CC, paramCodes,
        internal::Parameter<Ts>(vs)...
    );

    if (call(internal::parseArgumentsHelper<internal::Parameter<Ts>...>, params)) {
        call(internal::parseArgumentsCommiter<internal::Parameter<Ts>...>, params);
        return true;
    }

    return false;
}

template <typename... Ts>
    bool parseArguments(
        int ht TSRMLS_DC,
        std::tuple<Ts...> &vs
    )
{
    auto vRefs = construct<std::tuple<Ts&...>>(vs);

    auto params = std::tuple_cat(
        std::make_tuple(ht TSRMLS_CC),
        vRefs
    );

    return call(
        static_cast<bool (*)(int ht TSRMLS_DC, Ts&...)>(&parseArguments<Ts...>),
        params
    );
}

}
}

#endif /* end of include guard */
