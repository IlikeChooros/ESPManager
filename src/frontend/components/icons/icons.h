#pragma once

#include <TFT_eSPI.h>

#include "../draw/components.h"

namespace ui
{
    class Icon
    {
    public:
        Icon() = default;

        virtual void
        draw(
            int16_t x, int16_t y,
            int16_t size, uint16_t color,
            uint16_t bg
        ) {}
    };

    class SettingsIcon : public Icon
    {
    public:
        SettingsIcon() = default;

        void
        draw(
            int16_t x, int16_t y,
            int16_t size, uint16_t color,
            uint16_t bg
        );
    };
}