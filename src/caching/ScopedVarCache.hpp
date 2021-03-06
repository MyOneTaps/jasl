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
    using RealArray = std::vector<double>;
    using ByteArray = std::vector<uint8_t>;
    using StringArray = std::vector<std::string>;
    using OptionalInt = ::boost::optional<int64_t>;
    using OptionalBool = ::boost::optional<bool>;
    using OptionalReal = ::boost::optional<double>;
    using OptionalString = ::boost::optional<std::string>;
    using OptionalList = ::boost::optional<List>;
    using OptionalIntArray = ::boost::optional<IntArray>;
    using OptionalRealArray = ::boost::optional<RealArray>;
    using OptionalByteArray = ::boost::optional<ByteArray>;
    using OptionalByte = ::boost::optional<uint8_t>;
    using CacheVariant = ::boost::variant<int64_t, 
                                          bool, 
                                          double,
                                          uint8_t, 
                                          std::string, 
                                          List,
                                          IntArray,
                                          RealArray,
                                          ByteArray,
                                          StringArray >;

    /// Represents the type of a cached variable entry
    enum class Type {
        Int, Bool, Real, Byte, String, List,
        IntArray, RealArray, ByteArray, StringArray, None
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

    public:
        ScopedVarCache();

        /// functions for setting different types
        template <typename T> 
        void setVar(std::string const & key,
                    T const & value,
                    Type const type);

        /// sets var if exists
        /// return true if could be set
        template <typename T> 
        bool setVarIfExists(std::string const & key,
                            T const & value,
                            Type const type);

        /// Returns true if list existed
        bool setTokenInList(std::string const &key,
                            int const index,
                            Value const &value);

        /// Returns true if list existed
        bool pushBackTokenInList(std::string const &key,
                                 Value const &value);

        /// array support
        template <typename V, typename T>
        bool setValueInArray(std::string const & key,
                             int const index,
                             V const value);

        template <typename V, typename T>
        bool pushBackValueInArray(std::string const & key,
                                  V const value);

        bool eraseValue(std::string const &key);

        /// functions for getting different types.
        /// These are convenience functions and
        /// shouldn't be used unless it is known values exist.
        template <typename T>
        ::boost::optional<T> getVar(std::string const & key, Type const type);
        template <typename T>
        bool getVar_(std::string const & key, T & value, Type const type);

        Value getListToken(std::string const &key, size_t const index);
        OptionalType getType(std::string const &key);

        template <typename V>
        ::boost::optional<typename V::value_type> getArrayValue(std::string const & key, 
                                                                size_t const index,
                                                                Type const type);
        
    };

}