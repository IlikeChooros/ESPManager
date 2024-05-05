#include "Tokenizer.h"
 
BEGIN_LAZY_JSON_NAMESPACE

std::string verboseTokenType(TOKEN_TYPE type)
{
    switch (type)
    {
    case TOKEN_TYPE::CURLY_OPEN:
        return "CURLY_OPEN";
    case TOKEN_TYPE::CURLY_CLOSE:
        return "CURLY_CLOSE";
    case TOKEN_TYPE::ARRAY_OPEN:
        return "ARRAY_OPEN";
    case TOKEN_TYPE::ARRAY_CLOSE:
        return "ARRAY_CLOSE";
    case TOKEN_TYPE::COMMA:
        return "COMMA";
    case TOKEN_TYPE::COLON:
        return "COLON";
    case TOKEN_TYPE::STRING:
        return "STRING";
    case TOKEN_TYPE::NUMBER:
        return "NUMBER";
    case TOKEN_TYPE::BOOLEAN:
        return "BOOLEAN";
    case TOKEN_TYPE::NULL_TYPE:
        return "NULL_TYPE";
    default:
        return "UNKNOWN";
    }
}

Tokenizer::Tokenizer(const char *data)
{
    setData(data);
}

void Tokenizer::setData(const char *data)
{
    _prevPos = 0;
    _stream.set((char *)data);
}

std::string Tokenizer::json(int start, int end)
{
    if (end > _stream.size()){
        end = _stream.size();
    }
    while (end < 0){
        end += _stream.size();
    }
    if (start > end){
        std::swap(start, end);
    }
    return std::string(_stream.data() + start, end - start);
}

void Tokenizer::setPos(size_t pos)
{
    _stream.seekg(pos, stream_pos::base);
}

void Tokenizer::validatePos(size_t &pos)
{
    _prevPos = _stream.tellg();

    _stream.seekg(pos, stream_pos::base);
    static_cast<void>(getWithoutWhiteSpace());
    pos = _stream.tellg();

    _stream.seekg(_prevPos);
}

size_t Tokenizer::getPos()
{
    return _stream.tellg();
}

bool Tokenizer::isWhiteSpace(const char &c)
{
    return (c == ' ' || c == '\n');
}

bool Tokenizer::isNumber(const char &c)
{
    return c >= '0' && c <= '9';
}

bool Tokenizer::isPartOfNumber(const char &c)
{
    return c == '-' || isNumber(c);
}

bool Tokenizer::hasTokens()
{
    return !_stream.eof();
}

char Tokenizer::getWithoutWhiteSpace()
{
    char c = ' ';
    while (isWhiteSpace(c))
    {
        _stream.get(c);
        if (!_stream.good())
        {
            if (isWhiteSpace(c))
            {
                throw std::runtime_error("Tokenizer::getWithoutWhiteSpace(): Run out of tokens");
            }
            return c;
        }
    }
    return c;
}

void Tokenizer::rollBack()
{
    if (_stream.eof())
    {
        _stream.clear();
    }
    _stream.seekg(_prevPos);
}

Token Tokenizer::getToken()
{
    if (!hasTokens())
    {
        throw std::runtime_error("Tokenizer::getToken(): No more tokens");
    }
    _prevPos = _stream.tellg();
    char c = getWithoutWhiteSpace();

#if DEBUG_JSON
    Serial.printf("PARSING: %c \n", c);
#endif

    Token token;
    switch (c)
    {
    // string reading
    case '"':
        token.type = TOKEN_TYPE::STRING;
        // token.value = "";
        _stream.get(c);
        while (c != '"')
        {
            token.value += c;
            _stream.get(c);
        }
        break;
    case '{':
        token.type = TOKEN_TYPE::CURLY_OPEN;
        break;
    case '}':
        token.type = TOKEN_TYPE::CURLY_CLOSE;
        break;
    // Expecting false
    case 'f':
        token.type = TOKEN_TYPE::BOOLEAN;
        token.value = "false";
        // std::ios_base
        _stream.seekg(4, stream_pos::cur);
        break;
    // Expecting true
    case 't':
        token.type = TOKEN_TYPE::BOOLEAN;
        token.value = "true";
        _stream.seekg(3, stream_pos::cur);
        break;
    // Expecting null
    case 'n':
        token.type = TOKEN_TYPE::NULL_TYPE;
        _stream.seekg(3, stream_pos::cur);
        break;
    case '[':
        token.type = TOKEN_TYPE::ARRAY_OPEN;
        break;
    case ']':
        token.type = TOKEN_TYPE::ARRAY_CLOSE;
        break;
    case ':':
        token.type = TOKEN_TYPE::COLON;
        break;
    case ',':
        token.type = TOKEN_TYPE::COMMA;
        break;
    // Number / Float parsing
    /*
    Numbers contain only digits, dots, and minus sign:
    - . 0 1 2 3 4 5 6 7 8 9

    Number should not start with '.'
    */
    default:
        if (isPartOfNumber(c))
        {
            token.type = TOKEN_TYPE::NUMBER;

            char peek;
            while (!_stream.eof())
            {
                token.value += c;

                peek = _stream.peek();

#if DEBUG_JSON
                Serial.printf("Curr: %c Peek: %c \n", c, peek);
#endif

                // look one ahead to see if that's a number
                if (!(isPartOfNumber(peek) || peek == '.'))
                {
                    break;
                }
                _stream.get(c);
            }
        }
        else
        {
            // Unknown token / invalid syntax in given json
            throw std::runtime_error(std::string("Unexpected token: ") + c +  "("+ 
                std::to_string(int(c)) +  ") at: " + std::to_string(_prevPos));
        }
    }
    return token;
}

Token Tokenizer::peekToken()
{
    Token token = getToken();
    rollBack();
    return token;
}

END_LAZY_JSON_NAMESPACE