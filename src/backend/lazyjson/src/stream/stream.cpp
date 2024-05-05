#include "stream.h"

BEGIN_LAZY_JSON_NAMESPACE

void stream::set(char *data)
{
    if (data != "")
    {
        _data = data;
        _state = GOOB_BIT;
        _end = strlen(_data);
    }
    else
    {
        _state = EOF_BIT;
    }
    _pos = 0;
}

void stream::get(char &c)
{
    if (eof() || !_data[_pos])
    {
        _state = EOF_BIT;
        c = EOF_BIT;
    }
    else
    {
        c = _data[_pos];
        _pos++;
    }
}

void stream::seekg(size_t pos, stream_pos type)
{
    switch (type)
    {
    case stream_pos::base:
        _pos = pos;
        break;
    case stream_pos::cur:
        _pos += pos;
        break;
    default:
        break;
    }
    clear();
    if (_pos >= _end)
    {
        _state = EOF_BIT;
        _pos = _end;
    }
}

size_t stream::tellg()
{
    return _pos;
}

char stream::peek()
{
    if (good() && _data[_pos] != 0)
    {
        return _data[_pos];
    }
    return BAD_BIT;
}

size_t stream::size(){
    return _end;
}

char* stream::data(){
    return _data;
}

void stream::clear(size_t state)
{
    _state = state;
}

bool stream::eof()
{
    return (_state & EOF_BIT) != 0;
}

bool stream::bad()
{
    return (_state & BAD_BIT) != 0;
}

bool stream::good()
{
    return _state == GOOB_BIT;
}

END_LAZY_JSON_NAMESPACE