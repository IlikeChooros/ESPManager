#pragma once

#include <TFT_eSPI.h>

#include "../data/Point.h"

#define VERTICAL_VAL 15
#define HORIZONTAL_VAL 20

#define MIN_HORIZONTAL 30
#define MIN_VERTICAL 25

#define TOUCH_VAL 100 

#define SLEEP_TIME 60000

enum class TouchPoint {
    null,
    left,
    right,
    up,
    down,
};


class TouchScreen
{
    TFT_eSPI* _tft;
    
    bool _lastState;
    bool _state;

    uint64_t _lastDebounceTime;

    void (*_on_left)();
    void (*_on_right)();
    void (*_on_up)();
    void (*_on_down)();
    void (*_on_sleep)();
    void (*_on_wakeup)();

    public:
    TouchScreen(
        TFT_eSPI *tft ,
        uint16_t* touchData
    );

    ///@brief Reads for pixel coordinates of touch
    /// @param point - reference to the point object
    /// @return true if screen is touched, false otherwise
    bool
    read(Point& point);

    
    /// @brief Will read the TFT screen, if touched, based on the screen fragmentation, the following fuctions will be called:
    /// on_left, on_right, on_up, on_down, if they werent set, nothing will happen
    TouchPoint
    read_touch();


    ///@brief Set function to be called when screens is touched on the left side
    /// @return reference to the object
    TouchScreen&
    on_left(void(*)(void));

    /// @brief Set function to be called when screens is touched on the right side 
    /// @return reference to the object
    TouchScreen& 
    on_right(void(*)(void));

    /**
     * @brief Set function to be called when screens is touched on the upper side
     * @return reference to the object
     */
    TouchScreen& 
    on_up(void(*)(void));

    /**
     * @brief Set function to be called when screens is touched on the down side
     * 
     * @return reference to the object
     */
    TouchScreen&
    on_down(void(*)(void));

    /**
     * @brief When screen isnt touched for at least 1 minute, the parsed function will be activated, only once in the 'sleep mode'
     * 
     * @return reference to the object
     */
    TouchScreen&
    on_sleep(void(*)(void));

    /**
     * @brief When in 'sleep mode', after touching the screen parsed function will be activated, note that other on touch function wont be activaed:
     * on_left, on_right etc.
     * 
     * @return reference to the object
     */
    TouchScreen& 
    on_wakeup(void(*)(void));

};