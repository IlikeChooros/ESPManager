#pragma once

#include <Arduino.h>

constexpr uint16_t DEFAULT_BTN_COLOR = 0x31C6;

class
TouchButton
{
    public:
    /**
     * @brief Virtual class that helps with creating personalized touchscreen button
     * 
     * @param x 
     * @param y 
     * @param width 
     * @param height 
     */
    explicit 
    TouchButton(
        int16_t x, 
        int16_t y, 
        int16_t width, 
        int16_t height
    );

    virtual
    ~TouchButton() = default;

    /**
     * @brief This function will be called before on_touch(), after the button area gets touched
     * 
     */
    TouchButton&
    addOnClickEvent(std::function<void(void)>);

    /**
     * @brief Checks wheter the button is touched
     * 
     * @param x 
     * @param y 
     * @return true 
     * @return false 
     */
    bool 
    check(
        int16_t x, 
        int16_t y
    );

    /**
     * @brief Set the color
     * 
     * @param color 
     */
    TouchButton&
    set_color(uint16_t color);

    virtual void 
    draw();
    
    virtual void 
    on_touch();

    uint16_t color;
    uint16_t bg;
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    std::function<void(void)> on_press;
};
