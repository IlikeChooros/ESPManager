#pragma once

#include "../components/icons/icons.h"

enum class direction{
    row,
    column
};

typedef struct {
    direction dir = direction::row;
    int x = 0;
    int y = 0;
    int spacing = 0;
} RenderProps;


enum class placement{
    start,
    middle,
    end
};

class Props
{
    public:
    Props(const placement&, uint16_t bg);

    placement location;
    int16_t x;
    int16_t y;
    uint16_t bg;
};

class TextProps: public Props
{
    public:
    TextProps(
        const String& text = "",
        const placement& = placement::start,
        const uint16_t& bg = TFT_BLACK,
        const char* font = "",
        uint16_t color = TFT_LIGHTGREY
    );

    const char* font;
    String text;
    uint16_t color;
};

class IconProps: public Props
{
    public:
    IconProps(const IconProps&);
    IconProps(
        ui::Icon* icon = nullptr,
        int16_t size = 30,
        uint16_t bg = TFT_BLACK,
        uint16_t color = TFT_WHITE,
        const placement& = placement::end
    );

    std::shared_ptr<ui::Icon> icon;
    int16_t size;
    uint16_t color;
};

void prepareButtonIconProps(
    int16_t x, int16_t y, 
    int16_t width, int16_t height, 
    IconProps* _icon_props
);

