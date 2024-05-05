#include "components.h"

void drawCard(
    uint16_t x, uint16_t y,
    uint16_t w, uint16_t h,
    uint16_t color, uint16_t bg,
    TFT_eSPI *tft
){
    tft->fillSmoothRoundRect(x, y, w, h, 5, color, bg);
}

void drawInputField(
    uint16_t x, uint16_t y,
    uint16_t w, uint16_t h,
    uint16_t color, uint16_t outline,
    uint16_t bg, TFT_eSPI *tft
){
    tft->fillSmoothRoundRect(x, y, w, h, 10, color, bg);
    tft->drawSmoothRoundRect(x - 2, y - 2, 12, 10, w + 4, h + 4, outline, bg);
}

void  
drawWiFi(
    uint16_t x, 
    uint16_t y, 
    uint8_t size, 
    uint8_t strength, 
    uint16_t bg,
    TFT_eSPI *tft
)
{
    // 0 - no conneciton
    // 1 - weak
    // 2 - medium
    // 3 - great
    // 4 - super

    uint16_t 
    first  = TFT_WHITE,
    second = 0x52AA,
    third  = 0x52AA,
    fourth = 0x52AA;
    switch (strength){
        case 4:
            fourth = TFT_WHITE;
        case 3:
            third = TFT_WHITE;
        case 2:
            second = TFT_WHITE;
        case 1:
            break;
        default: // no connection
            first = 0x52AA;
            break;
    }

    constexpr auto angleSize = 86;
    constexpr auto startAngle = 180 - angleSize / 2;
    constexpr auto endAngle = 180 + angleSize / 2;

    tft->drawSmoothArc(
        x + 0.5f*size, y + 0.8f*size,
         0.7f*size, 0.7f*size - 2,
          startAngle, endAngle, fourth,
          bg, true
    );

    tft->drawSmoothArc(
        x + 0.5f*size, y + 0.8f*size,
         0.5f*size, 0.5f*size - 2,
          startAngle, endAngle,
          third, bg, true
    );

    tft->drawSmoothArc(
        x + 0.5f*size, y + 0.8f*size, 
        0.3f*size, 0.3f*size - 2, 
        startAngle, endAngle, second,
        bg, true
    );

    tft->fillSmoothCircle(
        x + 0.5f*size, y + 0.8f*size,
        0.1f*size, first, bg
    );
}

constexpr auto angleSize = 250;
constexpr auto treshold = 360 - angleSize;

void drawLoadingState(
    uint16_t x, uint16_t y,
    uint16_t bg, uint16_t size,
    int16_t angle,
    TFT_eSPI *tft
){
    auto endAngle = angle + angleSize;
    if (angle > treshold){
        endAngle -= 360;
    }
    tft->drawSmoothArc(
        x + size*0.5f, y + size*0.5f, 
        size*0.5f - 1, size*0.5f - 4, 
        angle, endAngle, TFT_LIGHTGREY, bg
    );
}

void clearLoadingState(
    uint16_t x, uint16_t y,
    uint16_t bg, uint16_t size,
    int16_t angle,
    TFT_eSPI *tft
){
    auto endAngle = angle + angleSize;
    if (angle > treshold){
        endAngle -= 360;
    }
    tft->drawArc(x + size*0.5f, y + size*0.5f, size*0.5f, size*0.5f - 5, endAngle, angle, bg, bg, false);
}

void drawLocker(
    int16_t x, int16_t y,
    int16_t size, uint16_t bg, 
    bool unLocked,
    TFT_eSPI *tft
)
{
    constexpr uint16_t 
        lockedColor = 0xD491,
      unLockedColor = 0x9632;
    
    uint16_t color = unLocked ? unLockedColor : lockedColor;
    uint16_t angle = unLocked ? 120 : 60;

    tft->drawSmoothArc(
        x + 0.5f*size, y + 0.3f*size,
        0.25f*size, 0.2f*size,
        angle, 300, color, bg
    );

    tft->fillSmoothRoundRect(
        x + 0.10f*size, y + 0.4f*size, 
        0.8f*size, 0.6f*size, 3, 
        color, bg
    );
}