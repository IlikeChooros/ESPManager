#pragma once

#include <Arduino.h>


class ErrorProvider
{
    bool _caught = false;
    String _msg;
    String _code;
    public:

    inline void 
    catchFn(std::function<void(void)> throwableFn){
        _caught = false;
        try{
            throwableFn();
        }
        catch(const std::exception& e){
            _msg = e.what();
            _caught = true;
        }
    }

    inline bool 
    caught(){
        return _caught;
    }

    inline const String& 
    reason(){
        return _msg;
    }

    inline void 
    reason(const String& msg){
        _msg = msg;
    }

    inline const String&
    code() {
        return _code;
    }

    inline void 
    code(const String& code){
        _code = code;
    }

    inline void 
    log(){
        Serial.printf("Error: %s \n \t Code: %s", _msg.c_str(), _code.c_str());
    }
};