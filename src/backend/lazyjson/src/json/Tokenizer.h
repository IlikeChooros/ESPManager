#pragma once

#include "../stream/stream.h"
#include <stdexcept>
#include "../options.h"

BEGIN_LAZY_JSON_NAMESPACE

enum class TOKEN_TYPE
{
    CURLY_OPEN,
    CURLY_CLOSE,
    COLON,
    STRING,
    NUMBER,
    ARRAY_OPEN,
    ARRAY_CLOSE,
    COMMA,
    BOOLEAN,
    NULL_TYPE
};

typedef struct
{
    std::string value = std::string("");
    TOKEN_TYPE type = TOKEN_TYPE::NULL_TYPE;
    std::string toString()
    {
        return value;
    }
} Token;

class Tokenizer
{
    size_t _prevPos;
    

    bool isWhiteSpace(const char &c);
    bool isPartOfNumber(const char &c);
    bool isNumber(const char &c);

public:
    Tokenizer(const char *data = "");

    /// @brief Set the data to be tokenized
    void setData(const char *data = "");

    /// @brief Get the next token
    char getWithoutWhiteSpace();

    /// @brief Check if there are more tokens 
    bool hasTokens();

    /// @brief Roll back the tokenizer to the previous position (before the last getToken() call)
    void rollBack();

    /// @brief Set the position of the tokenizer
    void setPos(size_t pos);

    /// @brief Validate the position, update the postion if it's pointing to a white space
    void validatePos(size_t &pos);

    /// @brief Get the current position of the tokenizer
    size_t getPos();

    /// @brief Get the next token
    Token getToken();

    /// @brief Peek the next token
    Token peekToken();

    /// @brief Get the json string from start to end, supports negative end ( = _stream.size() + end),
    /// so -1 is the last character
    std::string json(int start = 0, int end = -1);

    stream _stream;
};

std::string verboseTokenType(TOKEN_TYPE type);

END_LAZY_JSON_NAMESPACE