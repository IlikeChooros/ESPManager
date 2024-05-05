#pragma once

/*

The whole idea of extractor is to filter the json string and store the positions of
the elements that are going to be parsed. This way, the parsing is done only when
the value is accessed. This is useful when the json string is big and only a few
elements are going to be used.

*/


#include <string>

#include "wrappers.h"


BEGIN_LAZY_JSON_NAMESPACE


/*

## Extractor

The `extractor` class allows to easily parse a JSON string. 
The parsing is done only when the value is accessed.

### Example

```cpp
using namespace lazyjson;

auto e = extractor(
    "{\"key\": "
        "{"
            "\"subkey\": [\"hello\", 1.5, true],"
            "\"subkey2\": 2,"
            "\"subkey3\": 3"
        "},"
    "\"key2\": \"value\"}"
);

e["key"]["subkey"][1].extract().as<double>(); // 1.5
e["key"]["subkey2"].extract().asInt(); // 2
e["key2"].extract().asString(); // "value"
e["random_key"].isNull(); // true
e["random_key"].extract().as<String>(); // ""
```

In the example above, the extracting was quite inefficient.
The parsing of the value at "key"->"subkey"->1 was done 3 times.
You can omit the parsing by caching the value.

However, caching the value can be done to optimize the parsing.

Caching example:

```cpp
using namespace lazyjson;
auto e = extractor(
    "{\"key\": "
        "{"
            "\"subkey\": [\"hello\", 1.5, true],"
            "\"subkey2\": 2,"
            "\"subkey3\": 3"
        "},"
    "\"key2\": \"value\"}"
);

e["key"].cache(); // cache the object at "key"
e["subkey"].extract().asInt(); // 1
e["subkey2"].extract().asInt(); // 2
e["subkey3"].extract().asInt(); // 3

e["subkey"].cache(); // cache the list at "subkey"
e[0].extract().as<String>(); // "hello"
e[1].extract().as<float>(); // 1.5
e[2].extract().as<bool>(); // true

e.reset(); // reset the parsing json string to initial state (the whole json object)
e["key2"].extract().asString(); // "value"
```

The memory footprint of the `extractor` class is very low,
it doesn't allocate any memory on the heap, and doesn't copy the json string
(except for the `cache()` method, which stores the cached json string).

*/
class extractor
{
    char* _data;
    std::string _json;
    int _start;
    int _end;
    int _cache_start;
    Tokenizer _tokenizer;
    bool _is_null;

    LazyType _instance_type();
    void _validate(const LazyType &expected);
    void _reset_cache();
    void _set_cache();
public:
    extractor(const char *json);
    ~extractor();

    /// @brief Resets the start and end positions of the parsing.
    /// Resets the json string to the initial state.
    void reset();

    /// @brief Sets the initial json string.
    extractor &set(const char *json);

    /*
    The `cache()` method is used to store the current parsing value.
    This is useful when the value is going to be accessed multiple times,
    since the extractor works similarly to a text filter, each time a value
    is accessed, the parsing is done again. The negative effects, such as
    slower parsing, can be avoided by caching the value. This way, the parsing
    is done minimum times.

    Overrides the previous cached value and sets the cached json string as the current json string.
    Caching can be done on any value, including objects, lists, numbers, strings, etc.
    

    Example:

    ```

    auto e = extractor(
    "{\"key\": "
        "{"
            "\"subkey\": [\"hello\", 1.5, true],"
            "\"subkey2\": 2,"
            "\"subkey3\": 3"
        "},"
    "\"key2\": \"value\"}"
    );

    e["key"].cache(); // cache the object at "key"

    e["subkey"].extract().asInt(); // 1
    e["subkey2"].extract().asInt(); // 2
    e["subkey3"].extract().asInt(); // 3

    e["subkey"].cache(); // cache the list at "subkey"

    e[0].extract().as<String>(); // "hello"
    e[1].extract().as<float>(); // 1.5
    e[2].extract().as<bool>(); // true

    e.reset(); // reset the parsing json string to initial state (the whole json object)

    e["key2"].extract().asString(); // "value"
    ```

    */ 
    void cache();

    /// @brief Returns the `cached` json string.
    const std::string& json();

    /// @brief Filters the JSON string by a key, the result is not extracted (parsed), 
    /// to get the value use the `extract()` method. If the key is not found, nothing happens and
    /// calling `extract()` will return this json object (since this is the value that was filtered)
    /// @param key 
    /// @throw `json::lazy::invalid_type` if the value is not an object.
    /// @return *this
    extractor &filter(const std::string &key);

    /// @brief Filters the JSON string by an index, the result is not extracted (parsed), 
    /// to get the value use the `extract()` method. If the index is out of range, nothing happens and
    /// calling `extract()` will return this json list (since this is the value that was filtered)
    /// @param index
    /// @throw `json::lazy::invalid_type` if the value is not a list.
    /// @return *this
    extractor &filter(int index);

    /// @brief Filters the JSON string by a key, same as `filter(const std::string &key)`
    extractor &operator[](const std::string &key);

    /// @brief Filters the JSON string by an index, same as `filter(int index)`
    extractor &operator[](int index);

    /*
    Extract the current filtered value. The value is parsed and returned as a wrapper.
    See also `cache()` method for caching the value and optimizing the parsing.

    Example:

    ```
    auto e = extractor(
        "{\"key\": "
            "{"
                "\"subkey\": [\"hello\", 1.5, true],"
                "\"subkey2\": 2,"
                "\"subkey3\": 3"
            "},"
        "\"key2\": \"value\"}"
    );

    e["key"]["subkey"][1].extract().as<double>(); // 1.5
    e["key"]["subkey2"].extract().asInt(); // 2
    e["key2"].extract().asString(); // "value"
    ```
    
    */
    wrapper extract();

    /*
    Checks wheter the value was not found.

    Example:

    ```
    
    extractor ex(
        "{\"key\": "
            "{"
                "\"subkey\": [\"hello\", 1.5, true],"
                "\"subkey2\": 2,"
                "\"subkey3\": 3"
            "},"
        "\"key2\": \"value\"}"
    );

    ex["random_key"].isNull(); // true
    ex["key"].isNull(); // false
    ex["key"]["subkey"][3].isNull(); // true
    ex["key"]["subkey"][0].isNull(); // false
    ex["key"]["subkey"][5]["hello"].isNull(); // true
    ex["key2"].isNull(); // false

    ```
    
    */
    bool isNull();
};


END_LAZY_JSON_NAMESPACE