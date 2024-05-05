#pragma once

#include <WiFi.h>
#include <vector>
#include "../../data/WifiInfo.h"

class WiFiProvider
{
    String _ssid;
    String _pass;
    bool _startConnection;
    bool _startScan;
    int _scannedNetworks;
    std::vector<WifiInfo> _wifis;
    std::function<void(void)> _scanCallback;
    std::function<void(void)> _connectCallback;
    public:
    WiFiProvider();

    /// @brief Acts as signal to start the scan, will be executed async
    void
    startScan();

    /// @brief Scans networks on the same core as the task is running on
    void
    linearScan();

    bool 
    isScanning();

    /// @brief Sets the callback function, which will be called after completing the scan, 
    /// user must call periodically `scanCompleted()` in order to the callback function get envoked
    /// @param  callback
    void 
    setScanCallback(std::function<void(void)>);

    /// @brief Sets the callback function, which will be called after successful connection to wifi
    /// @param  callback
    void 
    setConnectionCallback(std::function<void(void)>);

    /// @brief Check if the scan is completed, must be called after `startScan()`,  
    /// @return if the scan is completed, then callback function is called
    bool 
    scanCompleted();

    /// @brief Copies from WiFi class info about wifi to vector
    /// @return this pointer
    WiFiProvider*
    setScannedWifis();

    /// @return pointer to vector of scanned wifis
    std::vector<WifiInfo>*
    wifis();

    void
    prepareToConnect(const String& ssid, const String& pass);

    void
    connect();

    bool 
    connected();

    bool 
    isConnecting();

    void 
    reset();
};