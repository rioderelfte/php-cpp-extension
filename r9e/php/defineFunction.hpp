// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_PHP_DEFINEFUNCTION_HPP_INCLUDED
#define R9E_PHP_DEFINEFUNCTION_HPP_INCLUDED

#include <boost/optional.hpp>

#include <r9e/pp.hpp>
//#include <r9e/php/parameters.hpp>

namespace r9e {
namespace php {
namespace internal {

#define R9E_DEFINE_FUNCTION_IMPL_PARAMTYPE(type, name)            \
    ::r9e::php::internal::Parameter<type>::ParameterType name   \
    /**/

#define R9E_DEFINE_FUNCTION_IMPL_TYPE(type, name)            \
    type                                                   \
    /**/

#define R9E_DEFINE_FUNCTION_IMPL_VALUEARG(type, name)        \
    ::r9e::php::valueArgument(#name)                       \
    /**/

#define R9E_DEFINE_FUNCTION_IMPL_IMPLFUN(RETURN_TYPE, NAME, PARAMS)     \
    RETURN_TYPE NAME ## Impl(                                           \
        R9E_PP_SEQ_FOR_EACH_KOMMA(                                      \
            R9E_DEFINE_FUNCTION_IMPL_PARAMTYPE,                         \
            PARAMS                                                      \
        )                                                               \
    )                                                                   \
    /**/

#define R9E_DEFINE_FUNCTION_IMPL_CALLFUN(RETURN_TYPE, NAME, PARAMS)   \
    extern "C" void NAME ## Caller(INTERNAL_FUNCTION_PARAMETERS)      \
    {                                                                 \
        std::tuple<                                                   \
            R9E_PP_SEQ_FOR_EACH_KOMMA(                                \
                R9E_DEFINE_FUNCTION_IMPL_TYPE,                        \
                PARAMS                                                \
            )                                                         \
        > params;                                                     \
                                                                      \
        if (!::r9e::php::parseArguments(                              \
                ZEND_NUM_ARGS() TSRMLS_CC,                            \
                params)                                               \
           )                                                          \
            return;                                                   \
                                                                      \
        ::r9e::php::ZValue ret(return_value);                         \
        ret = ::r9e::call(NAME ## Impl, params);                      \
        return;                                                       \
    }                                                                 \
    /**/

#define R9E_DEFINE_FUNCTION_IMPL_FUNENTRY(RETURN_TYPE, NAME, PARAMS)  \
    ::r9e::php::FunctionEntry NAME(                                   \
        #NAME,                                                        \
        NAME ## Caller,                                               \
        {                                                             \
            R9E_PP_SEQ_FOR_EACH_KOMMA(                                \
                R9E_DEFINE_FUNCTION_IMPL_VALUEARG,                    \
                PARAMS                                                \
            )                                                         \
        }                                                             \
    )                                                                 \
    /**/

}

#define R9E_DEFINE_FUNCTION(RETURN_TYPE, NAME, PARAMS)            \
    R9E_DEFINE_FUNCTION_IMPL_IMPLFUN(RETURN_TYPE, NAME, PARAMS);  \
    R9E_DEFINE_FUNCTION_IMPL_CALLFUN(RETURN_TYPE, NAME, PARAMS)   \
    R9E_DEFINE_FUNCTION_IMPL_FUNENTRY(RETURN_TYPE, NAME, PARAMS); \
    R9E_DEFINE_FUNCTION_IMPL_IMPLFUN(RETURN_TYPE, NAME, PARAMS)   \
    /**/

}
}

#endif /* end of include guard */
