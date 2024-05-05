#include "errors.h"

BEGIN_LAZY_JSON_NAMESPACE

std::string lazyTypeError(const LazyTypedValues& node, LazyType type){
    return "expected " + verboseLazyType(type) + " but got " + verboseLazyType(node.type);
}

context_not_found::context_not_found(const std::string& key)
    : std::runtime_error("context not found: " + key)
{}

index_out_of_range::index_out_of_range(int index)
    : std::runtime_error("index out of range: " + std::to_string(index))
{}

invalid_type::invalid_type(const LazyType& expected, LazyType type)
    : std::runtime_error("expected " + verboseLazyType(expected) + " but got " + verboseLazyType(type))
{}

END_LAZY_JSON_NAMESPACE