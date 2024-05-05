#include "wrappers.h"


BEGIN_LAZY_JSON_NAMESPACE

wrapper::wrapper(LazyTypedValues value) {
    this->_value = value;
}

wrapper::wrapper(const wrapper& other) {
    static_cast<void>(operator=(other));
}

wrapper& wrapper::operator=(const wrapper& other) {

#if DEBUG_LAZY_JSON
    Serial.println("Copying wrapper");
#endif

    auto values = other._value.values;
    auto type = other._value.type;
    // clean up the current values
    destroyLazyValue(_value.values, _value.type);
    // copy the new values
    deepCopyLazyValue(values, type, _value.values);
    _value.type = type;
    _value.repr = other._value.repr;
    return *this;
}

wrapper::~wrapper(){
#if DEBUG_LAZY_JSON
    Serial.println("Destroying wrapper");
#endif
    destroyLazyValue(_value.values, _value.type);
}

LazyType wrapper::type(){
    return _value.type;
}

LazyTypedValues& wrapper::raw(){
    return _value;
}

LazyObject& wrapper::object(){
    _assert_type(LazyType::OBJECT);
    return *(_value.values.object);
}

LazyList& wrapper::list(){
    _assert_type(LazyType::LIST);
    return *(_value.values.list);
}

int wrapper::asInt(){
    return as<int>();
}

float wrapper::asFloat(){
    return as<float>();
}

bool wrapper::asBool(){
    if (isNull()){
        return false;
    }
    _assert_type(LazyType::BOOL);
    return _value.values.boolean;
}

bool wrapper::isNull(){
    return _value.type == LazyType::NULL_TYPE;
}

String wrapper::asString(){
    if (isNull()){
        return String();
    }
    _assert_type(LazyType::STRING);
    return String(_value.values.string->str().c_str());
}

void wrapper::_assert_type(LazyType type){
    if(_value.type != type ){
        this->~wrapper();
        throw invalid_type(type, _value.type);
    }
}

END_LAZY_JSON_NAMESPACE