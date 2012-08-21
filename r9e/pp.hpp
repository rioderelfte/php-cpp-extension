// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_PP_HPP_INCLUDED
#define R9E_PP_HPP_INCLUDED

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>

#define R9E_PP_IMPL_SEQ_FOR_EACH_HEAD(data, elem)   \
    data elem                                       \
    /**/

#define R9E_PP_IMPL_SEQ_FOR_EACH_TAIL(r, data, elem)    \
    , data elem                                         \
    /**/

#define R9E_PP_SEQ_FOR_EACH_KOMMA(FUNCTION, SEQ)    \
    R9E_PP_IMPL_SEQ_FOR_EACH_HEAD(                  \
        FUNCTION,                                   \
        BOOST_PP_SEQ_HEAD(SEQ)                      \
    )                                               \
    BOOST_PP_SEQ_FOR_EACH(                          \
        R9E_PP_IMPL_SEQ_FOR_EACH_TAIL,              \
        FUNCTION,                                   \
        BOOST_PP_SEQ_TAIL(SEQ)                      \
    )                                               \
    /**/

#endif /* end of include guard */
