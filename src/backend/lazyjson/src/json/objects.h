#pragma once


#include "Tokenizer.h"
#include "../options.h"
#include "../namespaces.h"

#include <list>

BEGIN_LAZY_JSON_NAMESPACE

class LazyObject;
class LazyList;
class LazyString;

/// @brief Enum class for all possible lazy types. PARSE_IDX is used to store
/// the parsing position of the value, so it can be parsed when needed. The
/// rest of the types are used to store parsed values.
enum class LazyType{
    OBJECT,
    LIST,
    STRING,
    NUMBER,
    BOOL,
    NULL_TYPE,
    PARSE_IDX
};

/// @brief Union of all possible json values, used to store parsed values, 
/// creating lazy objects, lists and strings. Numbers and booleans are loaded
/// immediately into memory, while objects, lists and strings are loaded lazily.
typedef union {
    LazyObject *object = 0;
    LazyList *list;
    LazyString *string;
    float number;
    bool boolean;
    int parse_idx;
} LazyValues;

typedef struct {
    LazyValues values;
    LazyType type;
    std::string repr;
} LazyTypedValues;

typedef struct {
    std::string key;
    LazyValues values;
    LazyType type;
} ObjectData;

typedef struct {
    int index;
    LazyValues values;
    LazyType type;
} ListData;

/// @brief Base class for all lazy objects, lists and strings. Has start and end
/// position in the json string, used to parse the value when needed.
class LazyLike
{
public:
    virtual std::string json();
    virtual ~LazyLike() = default;
    LazyLike(Tokenizer *t = nullptr): 
        _start(0), _end(0), _tokenizer(t) {}
    LazyLike(int start, int end, Tokenizer *t) {
        push(start, end);
        _tokenizer = t;
    }
    void push(int start, int end);

    int _start;
    int _end;
    Tokenizer *_tokenizer;
};

/// @brief LazyObject is a lazy representation of a json object. When a key is
/// accessed, the value is parsed and stored in the object. Keeps a list of
/// keys and their parsing positions.
class LazyObject: public LazyLike
{
public:
    LazyObject(int start, int end, Tokenizer *t) : LazyLike(start, end, t) {};
    LazyObject(Tokenizer *t);
    LazyObject(const LazyObject& other);
    ~LazyObject();

    /// @brief Adds a key to the object, with the parsing position.
    void add(const std::string& key, int parse_idx);
    /// @brief Adds a key to the object, with the parsed value.
    void add(const std::string& key, LazyValues value, LazyType type);

    /// @brief Searches for the value at the given key and lazily parses it.
    /// @deprecated Use `extractor::filter(const std::string& key)` instead.
    LazyTypedValues operator[](const std::string& key);

    /// @brief Searches for the value at the given key and lazily parses it.
    /// @deprecated Use `extractor::filter(const std::string& key)` instead.
    LazyTypedValues get(const std::string& key, bool cache = false);

    /// @brief Deep copy of the `other` object.
    LazyObject& operator=(const LazyObject& other);

    std::list<ObjectData> _list;
};

/// @brief LazyList is a lazy representation of a json list. When an index is
/// accessed, the value is parsed and stored in the list. Keeps a list of
/// indexes and their parsing positions.

class LazyList : public LazyLike
{
public:
    LazyList(int start, int end, Tokenizer *t) : LazyLike(start, end, t) {};
    LazyList(Tokenizer *t);
    LazyList(const LazyList& other);
    ~LazyList();

    /// @brief Adds an index to the list, with the parsing position.
    void add(int index, int parse_idx);
    /// @brief Adds an index to the list, with the parsed value.
    void add(int index, LazyValues value, LazyType type);

    /// @brief Searches for the value at the given index and lazily parses it.
    /// @return Parsed value at the given index.
    /// @deprecated Use `extractor::filter(int index)` instead.
    LazyTypedValues operator[](const int& index);

    /// @brief Searches for the value at the given index and lazily parses it.
    /// @deprecated Use `extractor::filter(int index)` instead.
    LazyTypedValues get(const int& index, bool cache = false);

    /// @brief Deep copy of the `other` list.
    LazyList& operator=(const LazyList& other);

    std::list<ListData> _list;
};

/// @brief LazyString is a lazy representation of a json string. When the string
/// is accessed, the value is parsed. Doesn't store the value, only the parsing
/// position.
class LazyString : public LazyLike
{
public:
    LazyString(int start, int end, Tokenizer *t) : LazyLike(start, end, t) {};
    LazyString(Tokenizer *t): LazyLike(t) {};
    LazyString(const LazyString& other);
    LazyString& operator=(const LazyString& other);

    std::string str();
};

/// @brief Uses global Tokenizer to parse json string. Uses lazy parsing,
/// so it doesn't parse the whole string at once. Objects and lists 
/// are also parsed lazily, their values are not parsed (skipped).
/// @return parsed json object
LazyTypedValues lazy_parse(size_t pos, bool deep, Tokenizer *_tokenizer);

/// @brief Parses json object from the global Tokenizer. Only keys are evalueated,
/// values are skipped.
/// @param pos position in the json string
/// @param deep if true, values are also parsed
LazyObject *object_parse(size_t pos, bool deep, Tokenizer *_tokenizer);

/// @brief Parses json list from the global Tokenizer. Only indexes are evalueated,
/// values are skipped.
/// @param pos position in the json string
/// @param deep if true, values are also parsed
LazyList *list_parse(size_t pos, bool deep, Tokenizer *_tokenizer);

/// @brief Parses json string from the global Tokenizer. Onyl the parsing position
/// is stored.
/// @param pos 
/// @return LazyString*
LazyString *string_parse(size_t pos, Tokenizer *_tokenizer);

std::string verboseLazyType(LazyType type);

void fast_forward(size_t pos, TOKEN_TYPE begin,
                  TOKEN_TYPE end, Tokenizer *_tokenizer);

void deepCopyLazyValue(LazyValues& value, LazyType& type, LazyValues& dest);

void destroyLazyValue(LazyValues& value, LazyType& type);

END_LAZY_JSON_NAMESPACE