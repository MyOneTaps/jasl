//
//  ScopedVarCache.hpp
//  jasl
//
//  Created by Ben Jones 
//  Copyright (c) 2015-2016 Ben Jones. All rights reserved.
//

#pragma once

#include "Value.hpp"

#include <map>
#include <string>
#include <vector>
#include <cstdint>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace jasl {

    using IntArray = std::vector<int64_t>;
    using DoubleArray = std::vector<double>;
    using ByteArray = std::vector<uint8_t>;
    using OptionalInt = ::boost::optional<int64_t>;
    using OptionalBool = ::boost::optional<bool>;
    using OptionalDouble = ::boost::optional<double>;
    using OptionalString = ::boost::optional<std::string>;
    using OptionalValueArray = ::boost::optional<ValueArray>;
    using OptionalIntArray = ::boost::optional<IntArray>;
    using OptionalDoubleArray = ::boost::optional<DoubleArray>;
    using OptionalByteArray = ::boost::optional<ByteArray>;
    using OptionalByte = ::boost::optional<uint8_t>;
    using CacheVariant = ::boost::variant<int64_t, 
                                          bool, 
                                          double,
                                          uint8_t, 
                                          std::string, 
                                          ValueArray,
                                          IntArray,
                                          DoubleArray,
                                          ByteArray >;

    /// Represents the type of a cached variable entry
    enum class Type {
        Int, Bool, Double, Byte, String, ValueArray,
        IntArray, DoubleArray, ByteArray, None
    };

    using OptionalType = ::boost::optional<Type>;

    class ScopedVarCache 
    {
    public:
        /// For representing a variable entry in the 'big cache'
        struct CacheEntry {
            Type type;
            CacheVariant cv;
        };

    private:

        /// caches for ints, bools and doubles, and other types
        std::map<std::string, CacheEntry> m_bigCache;

        /// a separate parameter stack
        std::vector<CacheEntry> m_paramStack;

    public:
        ScopedVarCache();

        /// functions for setting different types
        void setInt(std::string const &key,
                    int64_t const value);
        void setByte(std::string const &key,
                     uint8_t const value);
        void setDouble(std::string const &key,
                       double const value);
        void setBool(std::string const &key,
                     bool const value);
        void setString(std::string const &key,
                       std::string const &value);
        void setList(std::string const &key,
                     ValueArray const &value);
        void setTokenInList(std::string const &key,
                            int const index,
                            Value const &value);
        void pushBackTokenInList(std::string const &key,
                                 Value const &value);

        /// int array support
        void setIntArray(std::string const & key,
                         IntArray const & array);
        void setValueInIntArray(std::string const & key,
                                int const index,
                                int64_t const value);
        void pushBackValueInIntArray(std::string const & key,
                                     int64_t const value);

        // double array support
        void setDoubleArray(std::string const & key,
                            DoubleArray const & array);
        void setValueInDoubleArray(std::string const & key,
                                   int const index,
                                   double const value);
        void pushBackValueInDoubleArray(std::string const & key,
                                        double const value);

        // byte array support
        void setByteArray(std::string const & key,
                          ByteArray const & array);
        void setValueInByteArray(std::string const & key,
                                 int const index,
                                 uint8_t const value);
        void pushBackValueInByteArray(std::string const & key,
                                      uint8_t const value);

        void eraseValue(std::string const &key);

        void resetParamStack();
        
        template <typename V>
        void addToParamStack(Type const type, V && value)
        {
            CacheEntry ce;
            ce.type = type;
            ce.cv = CacheVariant(std::forward<V>(value));
            m_paramStack.push_back(ce);
        }

        CacheEntry getParamFromStack(int const i);

        /// functions for getting different types.
        /// These are convenience functions and
        /// shouldn't be used unless it is know values exist.
        OptionalInt getInt(std::string const &key);
        bool getInt_(std::string const &key, int64_t &val);
        OptionalByte getByte(std::string const &key);
        bool getByte_(std::string const &key, uint8_t &val);
        OptionalDouble getDouble(std::string const &key);
        bool getDouble_(std::string const &key, double &val);
        OptionalBool getBool(std::string const &key);
        bool getBool_(std::string const &key, bool &val);
        OptionalString getString(std::string const &key);
        bool getString_(std::string const &key, std::string &val);
        OptionalValueArray getList(std::string const &key);
        bool getList_(std::string const &key, ValueArray &val);
        Value getListToken(std::string const &key, size_t const index);
        OptionalType getType(std::string const &key);

        OptionalIntArray getIntArray(std::string const &key);
        bool getIntArray_(std::string const &key, IntArray &val);
        OptionalInt getIntArrayValue(std::string const &key, size_t const index);

        OptionalDoubleArray getDoubleArray(std::string const &key);
        bool getDoubleArray_(std::string const &key, DoubleArray &val);
        OptionalDouble getDoubleArrayValue(std::string const &key, size_t const index);

        OptionalByteArray getByteArray(std::string const &key);
        bool getByteArray_(std::string const &key, ByteArray &val);
        OptionalByte getByteArrayValue(std::string const &key, size_t const index);


    };

}