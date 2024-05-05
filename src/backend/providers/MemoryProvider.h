#pragma once

#include <Arduino.h>
#include <vector>
#include "EEPROM.h"

constexpr int eepromSize = 512;

constexpr int networkSsidMaxLen = 20;
constexpr int networkPassMaxLen = 20;

constexpr int citiesMaxLen = 20;
constexpr int maxCitiesCount = 2;
constexpr int citiesStartAddress = eepromSize - maxCitiesCount * (citiesMaxLen + 2); 

constexpr int networkStartAddress = 10;
constexpr int maxNetworks = 
    (citiesStartAddress - 1 - networkStartAddress) / (networkSsidMaxLen + networkPassMaxLen + 2);



class MemoryProvider
{
    bool _safe;
    std::vector<std::pair<String, int>> _location;
    std::vector<std::pair<String, String>> _networks;

public:
    MemoryProvider();

    void 
    checkIntegrity(bool resetEeprom = false);

    void 
    readLocations();

    void 
    readNetworks();

    std::vector<std::pair<String, int>>*
    getLocations();

    /// @brief First -> ssid, second -> password
    std::vector<std::pair<String, String>>*
    getNetworks();

    bool 
    writeLocation(const String&, const int& index);

    bool 
    writeNetwork(const String& ssid, const String& pass);

    void 
    eraseNetwork(const String& ssid);

    void 
    eraseLocation(const String& loc);

    void 
    resetLocations();

    void 
    resetNetworks();
};