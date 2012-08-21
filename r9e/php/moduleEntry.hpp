// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_PHP_MODULEENTRY_HPP_INCLUDED
#define R9E_PHP_MODULEENTRY_HPP_INCLUDED

#include <r9e/php/functionEntry.hpp>

namespace r9e {
namespace php {

class ModuleEntry
{
public:
    ModuleEntry(
        const char *name,
        const char *version,
        FunctionEntrySequence functions
    )
    : functions_(std::move(functions))
    , moduleEntry_{
          STANDARD_MODULE_HEADER,
          name,
          functions_.functionEntry(),
          0, // module startup
          0, // module shutdown
          0, // request startup
          0, // request shutdown
          0, // info
          version,
          STANDARD_MODULE_PROPERTIES
      }
    {}

    ModuleEntry(
        const ModuleEntry &rhs
    )
    : functions_(rhs.functions_)
    , moduleEntry_{
          STANDARD_MODULE_HEADER,
          rhs.moduleEntry_.name,
          functions_.functionEntry(),
          0, // module startup
          0, // module shutdown
          0, // request startup
          0, // request shutdown
          0, // info
          rhs.moduleEntry_.version,
          STANDARD_MODULE_PROPERTIES
      }
    {}

    ModuleEntry(
        ModuleEntry &&rhs
    )
    : functions_(std::move(rhs.functions_))
    , moduleEntry_{
          STANDARD_MODULE_HEADER,
          rhs.moduleEntry_.name,
          functions_.functionEntry(),
          0, // module startup
          0, // module shutdown
          0, // request startup
          0, // request shutdown
          0, // info
          rhs.moduleEntry_.version,
          STANDARD_MODULE_PROPERTIES
      }
    {}

    ModuleEntry &operator=(const ModuleEntry &rhs) {
        functions_ = rhs.functions_;
        moduleEntry_ = rhs.moduleEntry_;
        moduleEntry_.functions = functions_.functionEntry();

        return *this;
    }

    ModuleEntry &operator=(ModuleEntry &&rhs) {
        functions_ = std::move(rhs.functions_);
        moduleEntry_ = rhs.moduleEntry_;
        moduleEntry_.functions = functions_.functionEntry();

        return *this;
    }

    const zend_module_entry *moduleEntry() const {
        return &moduleEntry_;
    }

private:
    FunctionEntrySequence functions_;
    zend_module_entry moduleEntry_;
};

}
}

#endif /* end of include guard */
