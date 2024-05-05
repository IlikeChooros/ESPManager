#pragma once

#include <Arduino.h>
#include "time.h"
// #include "../lang/localizers/Localizer.h"

constexpr auto CACHE_UPDATE_TIME = 15; // 15 mins

class DateTimeProvider
{
    struct tm _timeinfo;
    uint64_t _unix;

    uint64_t _lastTimeUpdate;

    bool _update;
    bool _initialized;
public:
    DateTimeProvider();

    /// @brief Requests to ntp server time info
    /// @param timezone in seconds
    void
    init(uint32_t timezone);

    /// @brief Fetches time from ntp server
    void fetchTime();

    /// @brief Wheter time is synced with ntp server
    bool synced();

    /// @brief Wheter this provider was initialized
    bool inited();

    /// @brief Adds 1 second every elapsed second
    /// @return true - date changed after adding second, false - only time changed
    bool addSecond();

    bool loop();

    struct tm* getTimeinfo();

    String formatDate();

    String formatTime(char* format = "%T");
};
