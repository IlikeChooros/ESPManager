#include "WiFiProvider.h"

extern WiFiProvider wifiProvider = WiFiProvider();

void doNothing(){
    return;
}

WiFiProvider::WiFiProvider(): _scanCallback(doNothing), _connectCallback(doNothing), _scannedNetworks(-1), _startScan(false) {}

void WiFiProvider::startScan(){
    _startScan = true;
}

bool WiFiProvider::isScanning(){
    return _startScan;
}

void WiFiProvider::setScanCallback(std::function<void(void)> callback){
    _scanCallback = callback;
}

void WiFiProvider::setConnectionCallback(std::function<void(void)> callback){
    _connectCallback = callback;
}

void WiFiProvider::linearScan(){
    if(!_startScan){
        return;
    }
    _scannedNetworks = WiFi.scanNetworks();
    if (_scannedNetworks < 0){
        return;
    }
    setScannedWifis();
    _scanCallback();
    _startScan = false;
}

bool WiFiProvider::scanCompleted(){
    auto scan = WiFi.scanComplete();
    if (scan < 0){
        return false;
    }
    _scannedNetworks = scan;
    setScannedWifis();
    _scanCallback();
    return true;
}


WiFiProvider* WiFiProvider::setScannedWifis(){
    if (_scannedNetworks < 0){
        return this;
    }
    _wifis.clear();
    _wifis.assign(_scannedNetworks, {String(), int8_t(0), false, String()});
    for (int i=0; i < _scannedNetworks; i++){
        _wifis.at(i) = {WiFi.SSID(i), int8_t(WiFi.RSSI(i)), false, String()};
    }
    return this;
}

std::vector<WifiInfo>*
WiFiProvider::wifis(){
    return &_wifis;
}

void WiFiProvider::
prepareToConnect(const String& ssid, const String& pass){
    _ssid = ssid;
    _pass = pass;
    _startConnection = true;
}

void WiFiProvider::
connect(){
    if (!_startConnection){
        return;
    }
    WiFi.begin(_ssid.c_str(), _pass.c_str());
    auto tries = 0, maxTries = 100;
    while(WiFi.status() != WL_CONNECTED && tries < maxTries){
        delay(100);
        tries++;
    }
    _connectCallback();
    _startConnection = false;
}

bool WiFiProvider::isConnecting(){
    return _startConnection;
}

bool WiFiProvider::
connected(){
    return WiFi.status() == WL_CONNECTED;
}

void WiFiProvider::reset(){
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}