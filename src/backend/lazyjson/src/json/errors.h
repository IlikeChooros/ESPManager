#pragma once

#include "objects.h"

BEGIN_LAZY_JSON_NAMESPACE

std::string lazyTypeError(const LazyTypedValues& node, LazyType type);

class context_not_found : public std::runtime_error
{
public:
    context_not_found(const std::string& key);
};

class index_out_of_range : public std::runtime_error
{
public:
    index_out_of_range(int index);
};

class invalid_type : public std::runtime_error
{
public:
    invalid_type(const LazyType& expected, LazyType type);
};

END_LAZY_JSON_NAMESPACE