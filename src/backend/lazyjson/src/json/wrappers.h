#pragma once

#include "errors.h"
#include "objects.h"
#include <type_traits>

#include <Arduino.h>

BEGIN_LAZY_JSON_NAMESPACE



class wrapper
{
    LazyTypedValues _value;
    void _assert_type(LazyType type);
public:
    wrapper() = default;
    wrapper(LazyTypedValues init);
    wrapper(const wrapper& other);
    ~wrapper();

    wrapper& operator=(const wrapper& other);

    LazyType type();
    LazyTypedValues& raw();

    LazyObject& object();
    LazyList& list();


    /*
        @brief Cast parsed value to type T
        @tparam T Type to cast to
        @throws `lazyjson::invalid_type` if the value cannot be casted to type T
        @return Value casted to type T
    */
    template<typename T>
    inline T as();

    /// @brief Cast parsed value to type int
    int asInt();

    /// @brief  Cast parsed value to type float
    float asFloat();
    
    /// @brief  Cast parsed value to type bool
    bool asBool();

    /// @brief  Cast parsed value to type String
    String asString();

    /// @brief  Check if value is null or does not exist 
    bool isNull();
};

template<typename T>
inline T wrapper::as()
{
    static_assert(std::is_arithmetic<T>::value, "wrapper::as<T>() : Type T must be an arithmetic type (int, bool, float, etc.)");
    if (std::is_same<T, bool>::value){
        return asBool();
    }
    if (std::is_floating_point<T>::value){
        return static_cast<T>(_value.values.number);
    }
    if (!_value.repr.empty()){
        return static_cast<T>(std::stoll(_value.repr));
    }
    return static_cast<T>(_value.values.number);
}

template<>
inline String wrapper::as<String>(){
    return asString();
}

template<>
inline std::string wrapper::as<std::string>(){
    return std::string(asString().c_str());
}

END_LAZY_JSON_NAMESPACE
