#pragma once

#include <Arduino.h>

class WifiInfo
{
    public:
    String ssid;
    int8_t strenght;
    bool isSaved;
    String pass;

    inline bool operator!=(const WifiInfo& _other) {
        return _other.ssid != this->ssid && _other.strenght != this->strenght;
    }
};