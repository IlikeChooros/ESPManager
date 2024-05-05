#include "Props.h"

Props::Props(const placement& loc, uint16_t bg): location(loc), x(0), y(0), bg(bg) {}

TextProps::TextProps(
    const String& text,
    const placement& loc,
    const uint16_t& bg,
    char* font,
    uint16_t color
): Props(loc, bg), font(font), text(text), color(color) {}

IconProps::IconProps(const IconProps& other): 
IconProps(nullptr, other.size, other.bg, other.color, other.location) {
    icon = other.icon;
}

IconProps::IconProps(
    ui::Icon* icon,
    int16_t size,
    uint16_t bg,
    uint16_t color,
    const placement& loc
): Props(loc, bg), icon(icon), size(size), color(color) {}

void prepareButtonIconProps(
    int16_t x, int16_t y, 
    int16_t width, int16_t height,
    IconProps* _icon_props
){
    constexpr uint8_t OFFSET_X = 5;
    int16_t icon_y = y, icon_x = x, size = _icon_props->size;

    // Calculate coordinates of icon
    icon_y = y + (height - size)/2;
    switch (_icon_props->location)
    {
        case placement::end:
            icon_x = x + width - size - OFFSET_X;
            break;
        case placement::middle:
            icon_x = x + (width - size)*0.5f;
            break;
        default:
            icon_x = x + width + OFFSET_X;
            break;
    }
    _icon_props->x = icon_x;
    _icon_props->y = icon_y;
}