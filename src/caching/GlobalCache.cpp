//
//  GlobalCache.cpp
//  jasl
//
//  Created by Ben Jones 
//  Copyright (c) 2015-2016 Ben Jones. All rights reserved.
//

#include "GlobalCache.hpp"
#include <iostream>

#define SET_VAR(X) \
template void GlobalCache::setVar(std::string const & key, X const & value, Type const type);
#define SET_VALUE_IN_ARRAY(V, T) \
template void GlobalCache::setValueInArray<V, T>(std::string const & key, int const index, V value);
#define PUSH_BACK_VALUE_IN_ARRAY(V, T) \
template void GlobalCache::pushBackValueInArray<V,T>(std::string const & key, V value);
#define GET_VAR(T) \
template ::boost::optional<T> GlobalCache::getVar<T>(std::string const&, Type const);
#define GET_VAR_(T) \
template bool GlobalCache::getVar_(std::string const & key, T & value, Type const type);
#define GET_ARRAY_VALUE(T) \
template ::boost::optional<typename T::value_type> GlobalCache::getArrayValue<T>(std::string const & key, \
                                                                                 size_t const index, \
                                                                                 Type const type); \

#define PUSH_PARAM(V) \
template void GlobalCache::pushParam<V>(Type const type, V && value); \
template void GlobalCache::pushParam<V&>(Type const type, V & value);

#define PUSH_RETURN_VALUE(V) \
template void GlobalCache::pushReturnValue<V>(Type const type, V && value); \
template void GlobalCache::pushReturnValue<V&>(Type const type, V & value);

namespace jasl {

    SharedVarCache GlobalCache::bigCache = std::make_shared<ScopedVarCache>();
    std::vector<std::string> GlobalCache::args;
    std::string GlobalCache::script;
    std::string GlobalCache::lastKnownError;
    std::deque<ScopedVarCache::CacheEntry> GlobalCache::m_paramStack;
    std::deque<ScopedVarCache::CacheEntry> GlobalCache::m_returnStack;
 
    template <typename T> 
    void GlobalCache::setVar(std::string const & key,
                                T const & value,
                                Type const type)
    {
        bigCache->setVar(key, value, type);
    }

    /// Explicit instantiations
    SET_VAR(int64_t);
    SET_VAR(uint8_t);
    SET_VAR(std::string);
    SET_VAR(List);
    SET_VAR(IntArray);
    SET_VAR(RealArray);
    SET_VAR(ByteArray);
    SET_VAR(StringArray);
    SET_VAR(bool);
    SET_VAR(double);

    void GlobalCache::setTokenInList(std::string const &key,
                                     int const index,
                                     Value const &value)
    {
        bigCache->setTokenInList(key, index, value);
    }

    void GlobalCache::pushBackTokenInList(std::string const &key,
                                          Value const &value)
    {
        bigCache->pushBackTokenInList(key, value);
    }

    template <typename V, typename T>
    void GlobalCache::setValueInArray(std::string const & key,
                                        int const index,
                                        V const value)
    {
        bigCache->setValueInArray<V, T>(key, index, value);
    }

    /// Explicit instantiations
    SET_VALUE_IN_ARRAY(int64_t, IntArray);
    SET_VALUE_IN_ARRAY(double, RealArray);
    SET_VALUE_IN_ARRAY(uint8_t, ByteArray);
    SET_VALUE_IN_ARRAY(std::string, StringArray);

    template <typename V, typename T>
    void GlobalCache::pushBackValueInArray(std::string const & key,
                                            V const value)
    {
        bigCache->pushBackValueInArray<V, T>(key, value);
    }

    /// Explicit instantiations
    PUSH_BACK_VALUE_IN_ARRAY(int64_t, IntArray);
    PUSH_BACK_VALUE_IN_ARRAY(double, RealArray);
    PUSH_BACK_VALUE_IN_ARRAY(uint8_t, ByteArray);
    PUSH_BACK_VALUE_IN_ARRAY(std::string, StringArray);

    void GlobalCache::eraseValue(std::string const &key)
    {
        bigCache->eraseValue(key);
    }

    template <typename T>
    ::boost::optional<T> GlobalCache::getVar(std::string const & key, Type const type)
    {
        return bigCache->getVar<T>(key, type);
    }

    /// Explicit instantiations
    GET_VAR(int64_t);
    GET_VAR(double);
    GET_VAR(bool);
    GET_VAR(uint8_t);
    GET_VAR(List);
    GET_VAR(IntArray);
    GET_VAR(RealArray);
    GET_VAR(ByteArray);
    GET_VAR(StringArray);
    GET_VAR(std::string);

    template <typename T>
    bool GlobalCache::getVar_(std::string const & key, T & value, Type const type)
    {
        return bigCache->getVar_(key, value, type);
    }

    /// Explicit template instantiations (--linkage issues).
    GET_VAR_(int64_t);
    GET_VAR_(uint8_t);
    GET_VAR_(std::string);
    GET_VAR_(List);
    GET_VAR_(IntArray);
    GET_VAR_(RealArray);
    GET_VAR_(ByteArray);
    GET_VAR_(StringArray);
    GET_VAR_(bool);
    GET_VAR_(double);

    template <typename T>
    ::boost::optional<typename T::value_type> 
    GlobalCache::getArrayValue(std::string const & key, 
                               size_t const index,
                               Type const type)
    {
        return bigCache->getArrayValue<T>(key, index, type);
    }

    /// Explicit instantiations
    GET_ARRAY_VALUE(IntArray);
    GET_ARRAY_VALUE(RealArray);
    GET_ARRAY_VALUE(ByteArray);
    GET_ARRAY_VALUE(StringArray);

    Value GlobalCache::getListToken(std::string const &key, size_t const index)
    {
        return bigCache->getListToken(key, index);
    }

    OptionalType GlobalCache::getType(std::string const &key)
    {
        return bigCache->getType(key);
    }

    template <typename V>
    void GlobalCache::pushParam(Type const type, V && value)
    {
        ScopedVarCache::CacheEntry ce;
        ce.type = type;
        ce.cv = CacheVariant(std::forward<V>(value));
        m_paramStack.push_back(ce);
    }

    ScopedVarCache::CacheEntry GlobalCache::popParam()
    {
        auto val = m_paramStack.front();
        m_paramStack.pop_front();
        return val;
    }

    // Explicit instantiations
    PUSH_PARAM(int64_t);
    PUSH_PARAM(uint8_t);
    PUSH_PARAM(std::string);
    PUSH_PARAM(List);
    PUSH_PARAM(IntArray);
    PUSH_PARAM(RealArray);
    PUSH_PARAM(ByteArray);
    PUSH_PARAM(StringArray);
    PUSH_PARAM(bool);
    PUSH_PARAM(double);

    template <typename V>
    void GlobalCache::pushReturnValue(Type const type, V && value)
    {
        ScopedVarCache::CacheEntry ce;
        ce.type = type;
        ce.cv = CacheVariant(std::forward<V>(value));
        m_returnStack.push_back(ce);
    }
    ScopedVarCache::CacheEntry GlobalCache::popReturnValue()
    {
        auto val = m_returnStack.back();
        m_returnStack.pop_front();
        return val;
    }

    // Explicit instantiations
    PUSH_RETURN_VALUE(int64_t);
    PUSH_RETURN_VALUE(uint8_t);
    PUSH_RETURN_VALUE(std::string);
    PUSH_RETURN_VALUE(List);
    PUSH_RETURN_VALUE(IntArray);
    PUSH_RETURN_VALUE(RealArray);
    PUSH_RETURN_VALUE(ByteArray);
    PUSH_RETURN_VALUE(StringArray);
    PUSH_RETURN_VALUE(bool);
    PUSH_RETURN_VALUE(double);

}