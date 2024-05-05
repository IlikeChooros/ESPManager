#include "MemoryProvider.h"

extern MemoryProvider memoryProvider = MemoryProvider();

MemoryProvider::MemoryProvider(): _safe(false) {}

void MemoryProvider::checkIntegrity(bool reset){
    
    EEPROM.begin(eepromSize);

    if (reset){
        for (int i = networkStartAddress; i < 512; i++){
            EEPROM.write(i, 0);
        }
       EEPROM.commit();
       EEPROM.end();
       return;
    }

    // Check sizes
    if (EEPROM.read(citiesStartAddress) > maxCitiesCount){
        EEPROM.write(citiesStartAddress, 0);
        EEPROM.commit();
    }

    if (EEPROM.read(networkStartAddress) > maxNetworks){
        EEPROM.write(networkStartAddress, 0);
        EEPROM.commit();
    }

    _safe = true;

    EEPROM.end();
}


void MemoryProvider::readLocations(){
    /* 
    
    By default: (Same for networks, but instead of index, there is String with ssid)

     (start)          <------  20  ----->
    |  468  |  469  |  470  | ... |  490  |  491  |  492  | ... |  512  |
       ^^^     ^^^     ^^^           ^^^     ^^^     ^^^           ^^^
    number    index   start         last     index   start         last
    of saved (uint8)  of loc.       char             loc.          char
    loc.              name          of loc.          name           (0)
    (ex. 2)                          name (0)
    */

    if (!_safe){
        checkIntegrity();
    }

    EEPROM.begin(eepromSize);

    int size = EEPROM.read(citiesStartAddress);

    _location.clear();
    _location.assign(size <= maxCitiesCount ? size : maxCitiesCount, std::pair<String, uint8_t>("null", 0));

    for(int i = 0, addr = citiesStartAddress; 
        i < size && i < maxCitiesCount && addr <= 512; 
            addr += 2 + citiesMaxLen){

        // Memory might be fragmented, because of erasing, checking if that's a valid location
        if (EEPROM.readString(addr + 2).isEmpty()){
            continue;
        }
        _location.at(i).second = EEPROM.read(addr + 1);
        _location.at(i).first = EEPROM.readString(addr + 2);
        i++;
    }

    EEPROM.end();
}

void MemoryProvider::readNetworks(){
    if (!_safe){
        checkIntegrity();
    }

    EEPROM.begin(eepromSize);

    auto size = EEPROM.read(networkStartAddress);

    _networks.clear();
    _networks.assign(size <= maxNetworks ? size : maxNetworks, std::pair<String, String>("null", "null"));

    for (int i = 0, addr = networkStartAddress + 1;
            i < size && i < maxNetworks && addr < citiesStartAddress;
                addr += 2 + networkSsidMaxLen + networkPassMaxLen){

        // Memory might be fragmented, because of erasing, checking if that's a valid ssid
        if (EEPROM.readString(addr).isEmpty()){
            continue;
        }
        _networks.at(i).first = EEPROM.readString(addr);
        _networks.at(i).second = EEPROM.readString(addr + networkSsidMaxLen + 1);
        i++;
    }

    EEPROM.end();
}

std::vector<std::pair<String, int>>* MemoryProvider::
getLocations(){
    return &_location;
}

std::vector<std::pair<String, String>>* MemoryProvider::
getNetworks(){
    return &_networks;
}


bool MemoryProvider::
writeLocation(const String& loc, const int& index){

    if (loc.length() >= citiesMaxLen){
        return false;
    }

    if (!_safe){
        checkIntegrity();
    }

    EEPROM.begin(eepromSize);

    auto size = EEPROM.read(citiesStartAddress);
    bool written = false;

    // Serial.println("writing location... " + loc);
    for(int i = 0, addr = citiesStartAddress; addr < 512; addr += 2 + citiesMaxLen){

        if (!EEPROM.readString(addr + 2).isEmpty()){
            // Serial.printf("Blocking: %s %i \n", EEPROM.readString(addr + 2).c_str(), addr);
            continue;
        }

        // is empty
        EEPROM.write(addr + 1, index);
        EEPROM.writeString(addr + 2, loc);
        EEPROM.write(citiesStartAddress, size + 1);
        // Serial.printf("Successfuly written %i", EEPROM.commit());
        written = true;
        break;
    }

    EEPROM.end();
    return written;
}

bool MemoryProvider::
writeNetwork(const String& ssid, const String& pass){
    if (ssid.length() >= networkSsidMaxLen || pass.length() >= networkPassMaxLen){
        return false;
    }

    if (!_safe){
        checkIntegrity();
    }

    EEPROM.begin(eepromSize);

    auto size = EEPROM.read(networkStartAddress);
    bool written = false;

    for(int i = 0, addr = networkStartAddress + 1; addr < citiesStartAddress; addr += 2 + networkSsidMaxLen + networkPassMaxLen){

        if (!(EEPROM.readString(addr).isEmpty() || EEPROM.readString(addr) == ssid)){
            continue;
        }

        // is empty or already saved (updating pass)

        EEPROM.writeString(addr, ssid);
        EEPROM.writeString(addr + networkSsidMaxLen + 1, pass);
        EEPROM.write(networkStartAddress, size + 1);
        EEPROM.commit();
        written = true;
        break;
    }

    EEPROM.end();
    return written;
}

void MemoryProvider::eraseLocation(const String& loc){
    if (!_safe){
        checkIntegrity();
    }

    EEPROM.begin(eepromSize);

    auto size = EEPROM.read(citiesStartAddress);

    for(int addr = citiesStartAddress; addr <= 512; addr += 2 + citiesMaxLen){


        // Linear search for given location
        if (EEPROM.readString(addr + 2) != loc){
            continue;
        }

        // Found location, erasing it
        EEPROM.writeString(addr + 2, "");
        EEPROM.write(citiesStartAddress, size - 1);
        EEPROM.commit();
        break;
    }

    EEPROM.end();
}

void MemoryProvider::eraseNetwork(const String& ssid){
    if (!_safe){
        checkIntegrity();
    }

    EEPROM.begin(eepromSize);

    auto size = EEPROM.read(networkStartAddress);

    for(int addr = networkStartAddress + 1; addr < citiesStartAddress; addr += 2 + networkPassMaxLen + networkSsidMaxLen){
        // Linear search for given ssid
        if (EEPROM.readString(addr) != ssid){
            continue;
        }

        // Found location, erasing it
        EEPROM.writeString(addr, "");
        EEPROM.write(networkStartAddress, size - 1);
        EEPROM.commit();
        break;
    }

    EEPROM.end();
}

void MemoryProvider::
resetLocations(){
    EEPROM.begin(eepromSize);

    for (int addr = citiesStartAddress; addr <= eepromSize; addr += 2 + citiesMaxLen){
        if (EEPROM.readString(addr + 2).isEmpty()){
            continue;
        }
        EEPROM.writeString(addr + 2, "");
    }
    EEPROM.write(citiesStartAddress, 0);
    EEPROM.commit();
    EEPROM.end();
}

void MemoryProvider::
resetNetworks(){
    EEPROM.begin(eepromSize);

    for(int addr = networkStartAddress + 1; addr < citiesStartAddress; addr += 2 + networkPassMaxLen + networkSsidMaxLen){
        // Search for valid networks
        if (EEPROM.readString(addr).isEmpty()){
            continue;
        }
        // Found network, erasing it
        EEPROM.writeString(addr, "");
    }
    EEPROM.write(networkStartAddress, 0);
    EEPROM.commit();
    EEPROM.end();
}
