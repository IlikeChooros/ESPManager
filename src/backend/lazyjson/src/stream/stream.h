#pragma once

#include <cstring>
#include <string>
#include "../namespaces.h"


BEGIN_LAZY_JSON_NAMESPACE

#define GOOB_BIT 0
#define BAD_BIT 1
#define EOF_BIT 2

enum stream_pos
{
    base,
    cur,
};

/// @brief Optimized stream, does not copy given char array, and doesn't delete it, a simple
/// wrapper for char array access
class stream
{
    char *_data;
    size_t _pos;
    size_t _end;
    int _state;

public:
    stream(char *data = "")
    {
        set(data);
    }

    void set(char *data);

    void get(char &c);

    void seekg(size_t pos, stream_pos type = stream_pos::base);
    size_t tellg();

    void clear(size_t state = 0);

    /// @brief Allows to 'peek' next value, works the same as `get(char& c)`, but doesn't move the cursor
    /// @return next char, if cursor is at the end of the stream, then returns `EOF_BIT`
    char peek();

    bool eof();
    bool bad();
    bool good();
    size_t size();
    char* data();
};


END_LAZY_JSON_NAMESPACE