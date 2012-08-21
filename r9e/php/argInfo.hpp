// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_PHP_ARGINFO_HPP_INCLUDED
#define R9E_PHP_ARGINFO_HPP_INCLUDED

#include <vector>

#include <php.h>

namespace r9e {
namespace php {
namespace internal {

class ArgInfo
{
public:
    constexpr ArgInfo(
        bool isOptional,
        const char *name,
        zend_uint name_len,
        const char *class_name,
        zend_uint class_name_len,
        zend_uchar type_hint,
        zend_bool allow_null,
        zend_bool pass_by_reference
    )
    : argInfo_{
          name,
          name_len,
          class_name,
          class_name_len,
          type_hint,
          allow_null,
          pass_by_reference
      }
    , isOptional_(isOptional)
    {}

    constexpr zend_arg_info argInfo() {
        return argInfo_;
    }

    constexpr bool isOptional() {
        return isOptional_;
    }

private:
    zend_arg_info argInfo_;
    bool isOptional_;
};

inline constexpr size_t countRequired()
{
    return 0;
}

template <class T, class... Ts>
    inline constexpr size_t countRequired(
        const T &arg,
        const Ts&... rest
    ) {
    return (arg.isOptional() ? 0 : 1) + countRequired(rest...);
}

}

template <
    size_t NAME_LEN
>
    inline constexpr internal::ArgInfo valueArgument(
        const char (&name)[NAME_LEN]
    ) {
    return internal::ArgInfo(
        /* is optional                 */ false,
        /* const char *name            */ name,
        /* zend_uint name_len          */ NAME_LEN - 1,
        /* const char *class_name      */ 0,
        /* zend_uint class_name_len    */ 0,
        /* zend_uchar type_hint        */ 0,
        /* zend_bool allow_null        */ false,
        /* zend_bool pass_by_reference */ false
    );
}

template <
    size_t NAME_LEN
>
    inline constexpr internal::ArgInfo optionalValueArgument(
        const char (&name)[NAME_LEN]
    ) {
    return internal::ArgInfo(
        /* is optional                 */ true,
        /* const char *name            */ name,
        /* zend_uint name_len          */ NAME_LEN - 1,
        /* const char *class_name      */ 0,
        /* zend_uint class_name_len    */ 0,
        /* zend_uchar type_hint        */ 0,
        /* zend_bool allow_null        */ false,
        /* zend_bool pass_by_reference */ false
    );
}

template <
    size_t NAME_LEN
>
    inline constexpr internal::ArgInfo referenceArgument(
        const char (&name)[NAME_LEN]
    ) {
    return internal::ArgInfo(
        /* is optional                 */ false,
        /* const char *name            */ name,
        /* zend_uint name_len          */ NAME_LEN - 1,
        /* const char *class_name      */ 0,
        /* zend_uint class_name_len    */ 0,
        /* zend_uchar type_hint        */ 0,
        /* zend_bool allow_null        */ false,
        /* zend_bool pass_by_reference */ true
    );
}

template <
    size_t NAME_LEN
>
    inline constexpr internal::ArgInfo optionalReferenceArgument(
        const char (&name)[NAME_LEN]
    ) {
    return internal::ArgInfo(
        /* is optional                 */ true,
        /* const char *name            */ name,
        /* zend_uint name_len          */ NAME_LEN - 1,
        /* const char *class_name      */ 0,
        /* zend_uint class_name_len    */ 0,
        /* zend_uchar type_hint        */ 0,
        /* zend_bool allow_null        */ false,
        /* zend_bool pass_by_reference */ true
    );
}

class ArgInfoSequence
{
public:
    template <class... Ts>
    ArgInfoSequence(
        const Ts&... args
    )
    : argInfo_{
          {
              /* const char *name            */ 0,
              /* zend_uint name_len          */ 0,
              /* const char *class_name      */ 0,
              /* zend_uint class_name_len    */ static_cast<zend_uint>(internal::countRequired(args...)),
              /* zend_uchar type_hint        */ 0,
              /* zend_bool allow_null        */ false,
              /* zend_bool pass_by_reference */ false
          },
          args.argInfo()...
      }
    {}

    void returnReference(bool val)
    {
        argInfo_[0].allow_null = val;
    }

    void passRestByReference(bool val)
    {
        argInfo_[0].pass_by_reference = val;
    }

    const zend_arg_info *argInfo() const
    {
        return &argInfo_[0];
    }

    size_t size() const
    {
        return argInfo_.size() - 1;
    }

private:

    std::vector<zend_arg_info> argInfo_;
};

}
}

#endif /* end of include guard */
