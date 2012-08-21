// Copyright Florian Sowade 2012.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef R9E_PHP_DATATYPES_HPP_INCLUDED
#define R9E_PHP_DATATYPES_HPP_INCLUDED

#include <boost/optional.hpp>

struct _zval_struct;

namespace r9e {
namespace php {

class ZValue;

class String
{
public:
    String()
    : String(STR_EMPTY_ALLOC(), 0, true)
    {}

    String(char *str, int length, bool needsFree)
    : length_(length)
    , data_(str)
    , needsFree_(needsFree)
    {}

    String(const std::string &rhs)
    : length_(rhs.size())
    , data_(estrndup(rhs.c_str(), length_))
    , needsFree_(true)
    {}

    template <size_t LENGTH>
        String(const char (&rhs)[LENGTH])
        : length_(LENGTH - 1)
        , data_(estrndup(rhs, length_))
        , needsFree_(true)
    {}

    String(const char *rhs)
    : length_(strlen(rhs))
    , data_(estrndup(rhs, length_))
    , needsFree_(true)
    {}

    ~String() {
        doFree();
    }

    String (const String&) = delete;
    String &operator=(const String&) = delete;

    String(String &&rhs)
    : String(rhs.data_, rhs.length_, rhs.needsFree_)
    {
        rhs.data_      = 0;
        rhs.length_    = 0;
        rhs.needsFree_ = false;
    }

    String &operator=(String &&rhs)
    {
        doFree();
        data_      = rhs.data_;
        length_    = rhs.length_;
        needsFree_ = rhs.needsFree_;

        rhs.data_      = 0;
        rhs.length_    = 0;
        rhs.needsFree_ = false;

        return *this;
    }

    std::string toString() const
    {
        return std::string(data_, length_);
    }

private:
    friend class ZValue;

    void doFree() {
        if (needsFree_) {
            efree(data_);
        }
    }

    int length_;
    char *data_;
    bool needsFree_;
};

class ZValue
{
public:
    enum Type {
        T_NULL     = IS_NULL,
        T_BOOL     = IS_BOOL,
        T_LONG     = IS_LONG,
        T_DOUBLE   = IS_DOUBLE,
        T_STRING   = IS_STRING,
        T_RESOURCE = IS_RESOURCE,
        T_ARRAY    = IS_ARRAY,
        T_OBJECT   = IS_OBJECT
    };

    ZValue()
    : zval_(0)
    {
        ALLOC_INIT_ZVAL(zval_);
    }

    ZValue(const ZValue &rhs)
    : zval_(rhs.zval_)
    {
        addRef();
    }

    ZValue(ZValue &&rhs)
    : zval_(rhs.zval_)
    {
        rhs.zval_ = 0;
    }

    ZValue(_zval_struct *zval)
    : zval_(zval)
    {
        Z_ADDREF_P(zval_);
    }

    template <typename T>
        ZValue(T &&val)
        : ZValue()
    {
        assign(std::forward<T>(val));
    }

    ~ZValue() {
        if (zval_) {
            decRef();
        }
    }

    ZValue &operator=(const ZValue &rhs) {
        return *this = rhs.zval_;
    }

    ZValue &operator=(_zval_struct *rhs) {
        ZVAL_ZVAL(zval_, rhs, true, false);

        return *this;
    }

    template <typename T>
        ZValue &operator=(T &&val)
    {
        assign(std::forward<T>(val));
        return *this;
    }

    const _zval_struct *get() const {
        return zval_;
    }

    _zval_struct *get() {
        return zval_;
    }

    Type type() const {
        return static_cast<Type>(Z_TYPE_P(zval_));
    }

    bool isReference() const {
        return Z_ISREF_P(zval_);
    }

    size_t refcount() const {
        return Z_REFCOUNT_P(zval_);
    }

    void toString() {
        convert_to_string(zval_);
    }

    void toLong() {
        convert_to_long(zval_);
    }

    void toLong(int base) {
        convert_to_long_base(zval_, base);
    }

    void toDouble() {
        convert_to_double(zval_);
    }

    void toNull() {
        convert_to_null(zval_);
    }

    void toBoolean() {
        convert_to_boolean(zval_);
    }

private:
    void addRef() {
        Z_ADDREF_P(zval_);
    }

    void decRef() {
        zval_ptr_dtor(&zval_);
    }

    template <typename T>
        void assign(const boost::optional<T> &val)
    {
        if (val) {
            assign(*val);
        } else {
            assign();
        }
    }

    void assign(bool val) {
        ZVAL_BOOL(zval_, val);
    }

    void assign(long val) {
        ZVAL_LONG(zval_, val);
    }

    void assign(const String &val) {
        ZVAL_STRINGL(zval_, val.data_, val.length_, true);
    }

    void assign(String &&val) {
        ZVAL_STRINGL(zval_, val.data_, val.length_, !val.needsFree_);
        val.data_      = 0;
        val.length_    = 0;
        val.needsFree_ = false;
    }

    void assign(const std::string &val) {
        ZVAL_STRINGL(zval_, val.c_str(), val.size(), true);
    }

    void assign(double val) {
        ZVAL_DOUBLE(zval_, val);
    }

    void assign() {
        ZVAL_NULL(zval_);
    }

    _zval_struct *zval_;
};

struct Resource
{
    ZValue zval;
};

struct Array
{
    ZValue zval;
};

struct Object
{
    ZValue zval;
};

}
}

#endif /* end of include guard */
