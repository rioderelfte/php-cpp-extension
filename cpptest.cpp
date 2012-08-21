// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>

#include <r9e/php/parameters.hpp>
#include <r9e/php/argInfo.hpp>
#include <r9e/php/moduleEntry.hpp>
#include <r9e/php/defineFunction.hpp>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

using namespace r9e::php;

extern "C" void hello_world(INTERNAL_FUNCTION_PARAMETERS)
{
    std::string name;
    bool flag;

    if (!parseArguments(ZEND_NUM_ARGS() TSRMLS_CC, name, flag))
        return;

    std::string ret{"Hello World!!!elf and hello to " + name};

    if (flag)
        ret += ", lol";

    RETURN_STRING(ret.c_str(), 1);
}

extern "C" void hello_world2(INTERNAL_FUNCTION_PARAMETERS)
{
    std::tuple<std::string, bool> params;

    if (!parseArguments(ZEND_NUM_ARGS() TSRMLS_CC, params))
        return;

    std::string ret{"Hello World!!!elf and hello to " + std::get<0>(params)};

    if (std::get<1>(params))
        ret += ", lol";

    RETURN_STRING(ret.c_str(), 1);
}

extern "C" ZEND_FUNCTION(helloWorld)
{
    const char *nameData;
    int nameLength;
    long flag;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sb", &nameData, &nameLength, &flag) == FAILURE)
        return;

    std::string name(nameData, nameLength);

    std::string ret{"Hello World!!!elf and hello to " + name};

    if (flag)
        ret += ", lol";

    RETURN_STRING(ret.c_str(), 1);
}

extern "C" ZEND_FUNCTION(helloWorld2)
{
    hello_world2(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

std::string helloWorld3Impl(
    internal::Parameter<std::string>::ParameterType name,
    internal::Parameter<bool>::ParameterType flag
);

extern "C" void helloWorld3Caller(INTERNAL_FUNCTION_PARAMETERS)
{
    std::tuple<std::string, bool> params;

    if (!parseArguments(ZEND_NUM_ARGS() TSRMLS_CC, params))
        return;

    ZValue ret(return_value);
    ret = r9e::call(helloWorld3Impl, params);
}

FunctionEntry helloWorld3(
    "helloWorld3",
    helloWorld3Caller,
    {
        valueArgument("name"),
        valueArgument("flag")
    }
);

std::string helloWorld3Impl(const std::string &name, bool flag)
{
    std::string ret{"Hello World!!!elf and hello to " + name};

    if (flag)
        ret += ", lol";

    return ret;
}

R9E_DEFINE_FUNCTION(
    boost::optional<std::string>,
    helloWorld4,
    ((std::string, name))((bool, flag))
)
{
    if (!flag)
        return boost::none;

    return "Hello World!!!elf and hello to " + name + ", lol";
}

R9E_DEFINE_FUNCTION(
    String,
    helloWorld6,
    ((bool, flag))
)
{
    if (flag)
        return {"hallo welt"};

    return {"foooooooo baaaaaaaar"};
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_helloWorld, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, flag)
	ZEND_ARG_INFO(0, foobar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_helloWorld2, 1, 0, 2)
	ZEND_ARG_INFO(0, foo)
	ZEND_ARG_INFO(0, bar)
ZEND_END_ARG_INFO()

const zend_function_entry cpptest_functions[] = {
	PHP_FE(helloWorld, arginfo_helloWorld)
	PHP_FE(helloWorld2, arginfo_helloWorld2)
	PHP_FE_END
};

zend_module_entry date_module_entry = {
    STANDARD_MODULE_HEADER,
    "cpptest",
    cpptest_functions,
    0, // module startup
    0, // module shutdown
    0, // request startup
    0, // request shutdown
    0, // info
    "0.1",
    STANDARD_MODULE_PROPERTIES
};

ModuleEntry moduleEntry(
    "cpptest",
    "0.1",
    {
        {
            "helloWorld",
            hello_world,
            {
                valueArgument("name"),
                valueArgument("flag"),
                optionalValueArgument("foobar")
            }
        },
        {
            "helloWorld2",
            hello_world2,
            {
                valueArgument("foo"),
                valueArgument("bar")
            },
            passRestByReference
        },
        helloWorld3,
        helloWorld4,
        helloWorld6
    }
);

extern "C" const zend_module_entry *get_module()
{
    //return &date_module_entry;
    return moduleEntry.moduleEntry();
}
