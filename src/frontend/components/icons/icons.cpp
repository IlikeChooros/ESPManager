#include "icons.h"

extern TFT_eSPI tft;

namespace ui
{
    void SettingsIcon::draw(
        int16_t x, int16_t y,
        int16_t size, uint16_t color,
        uint16_t bg
    )
    {
        drawCard(x, y, size, size, color, bg, &tft);

        uint16_t gear_color = 0x73AE;

        tft.fillSmoothCircle(x+0.5f*size, y+0.5f*size, 0.3f*size, gear_color, color);

        uint16_t 
        xc = x + size/2,
        yc = y + size/2,
        Ax = 0.75f * size + x,
        Ay = y + size/2,
        Bx = 0.9f*size + x,
        By = Ay; 

        for (uint8_t i=0; i<9; i++)
        {
            float angle = i * PI/4;
            uint16_t
                ax = (Ax - xc) * cosf(angle) - (Ay - yc) * sinf(angle) + xc,
                ay = (Ax - xc) * sinf(angle) + (Ay - yc) * cosf(angle) + yc,
                bx = (Bx - xc) * cosf(angle) - (By - yc) * sinf(angle) + xc,
                by = (Bx - xc) * sinf(angle) + (By - yc) * cosf(angle) + yc;

            tft.drawWideLine(ax, ay,bx , by, 0.15f * size, gear_color, color);
        }

        tft.drawArc(x + 0.5f*size, y + 0.5f*size, 0.3f*size, 0.1f*size, 0, 360, gear_color, color, false);
        tft.fillSmoothCircle(x+0.5f*size, y+0.5f*size, 0.2f*size, color, gear_color);
    }

    void WiFiIcon::draw(
        int16_t x, int16_t y,
        int16_t size, uint16_t color,
        uint16_t bg
    )
    {
        drawCard(x, y, size, size, color, bg, &tft);
        drawWiFi(x + 0.1f*size, y + 0.1f*size, 0.8f*size, 4, color, &tft);
    }


} // namespace ui