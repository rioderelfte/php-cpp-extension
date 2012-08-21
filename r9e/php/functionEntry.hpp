// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_PHP_FUNCTIONENTRY_HPP_INCLUDED
#define R9E_PHP_FUNCTIONENTRY_HPP_INCLUDED

#include <vector>

#include <php.h>

#include <r9e/variadicAlgorithms.hpp>
#include <r9e/php/argInfo.hpp>

namespace r9e {
namespace php {

class FunctionEntry
{
public:
    template <class... FUNS>
    FunctionEntry(
        const char *name,
        void (*handler)(INTERNAL_FUNCTION_PARAMETERS),
        ArgInfoSequence argInfos,
        const FUNS&... funs
    )
    : argInfos_(std::move(argInfos))
    , functionEntry_{
          name,
          handler,
          argInfos_.argInfo(),
          static_cast<zend_uint>(argInfos_.size()),
          0
      }
    , doNothing_(funs(*this)...)
    {}

    FunctionEntry(const FunctionEntry &rhs)
    : argInfos_(rhs.argInfos_)
    , functionEntry_{
          rhs.functionEntry_.fname,
          rhs.functionEntry_.handler,
          argInfos_.argInfo(),
          static_cast<zend_uint>(argInfos_.size()),
          rhs.functionEntry_.flags
      }
    {}

    FunctionEntry(FunctionEntry &&rhs)
    : argInfos_(std::move(rhs.argInfos_))
    , functionEntry_{
          rhs.functionEntry_.fname,
          rhs.functionEntry_.handler,
          argInfos_.argInfo(),
          static_cast<zend_uint>(argInfos_.size()),
          rhs.functionEntry_.flags
      }
    {}

    FunctionEntry &operator=(const FunctionEntry &rhs)
    {
        argInfos_ = rhs.argInfos_;
        functionEntry_ = rhs.functionEntry_;
        functionEntry_.arg_info = argInfos_.argInfo();

        return *this;
    }

    FunctionEntry &operator=(FunctionEntry &&rhs)
    {
        argInfos_ = std::move(rhs.argInfos_);
        functionEntry_ = rhs.functionEntry_;
        functionEntry_.arg_info = argInfos_.argInfo();

        return *this;
    }

    zend_function_entry functionEntry() const {
        return functionEntry_;
    }

    ArgInfoSequence &argInfos()
    {
        return argInfos_;
    }

private:
    ArgInfoSequence argInfos_;
    zend_function_entry functionEntry_;
    DoNothing doNothing_;
};

bool passRestByReference(FunctionEntry &entry)
{
    entry.argInfos().passRestByReference(true);
    return true;
}

bool returnReference(FunctionEntry &entry)
{
    entry.argInfos().returnReference(true);
    return true;
}

class FunctionEntrySequence
{
public:
    FunctionEntrySequence(
        std::initializer_list<FunctionEntry> funs
    )
    : functionEntries_(funs.begin(), funs.end())
    {
        for (const FunctionEntry &fun : functionEntries_) {
            zendEntries_.push_back(fun.functionEntry());
        }

        zendEntries_.push_back({0, 0, 0, 0, 0});
    }

    FunctionEntrySequence(
        const FunctionEntrySequence &rhs
    )
    : functionEntries_(rhs.functionEntries_)
    {
        for (const FunctionEntry &fun : functionEntries_) {
            zendEntries_.push_back(fun.functionEntry());
        }

        zendEntries_.push_back({0, 0, 0, 0, 0});
    }

    FunctionEntrySequence &operator=(const FunctionEntrySequence &rhs) {
        functionEntries_ = rhs.functionEntries_;

        zendEntries_.clear();
        for (const FunctionEntry &fun : functionEntries_) {
            zendEntries_.push_back(fun.functionEntry());
        }

        zendEntries_.push_back({0, 0, 0, 0, 0});
    }

    const zend_function_entry *functionEntry() const {
        return &zendEntries_[0];
    }

private:
    std::vector<FunctionEntry> functionEntries_;
    std::vector<zend_function_entry> zendEntries_;
};

}
}

#endif /* end of include guard */
