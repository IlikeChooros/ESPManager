#pragma once

#include "TextElement.h"
#include "IconElement.h"

namespace ui
{
  class WidgetElement: public ScreenElement
  {
    public:
    WidgetElement(
      int x, int y, 
      int size,
      TFT_eSPI* tft, 
      const IconProps& icon,
      const TextProps& text
    ): ScreenElement(tft), iconProps(icon), 
      textProps(text.text, placement::middle, icon.bg, FontData::useFont(fonts::detail)), 
      area(x, y, size, size) 
    {
      const int px = 0.1f * size, py = 0.1f * size;

      iconProps.x = x + px;
      iconProps.y = y;
      iconProps.size = size - 2*px;
      textProps.y = y + iconProps.size + py;
      textProps.x = x + size * 0.5f;
    }

    void draw() override{
      if(iconProps.icon){
        iconProps.icon->draw(iconProps.x, iconProps.y, iconProps.size, iconProps.color, iconProps.bg);
      }
      if(textProps.text){
        FontLoader<TFT_eSPI> loader(_tft, textProps);
        _tft->drawString(textProps.text, textProps.x, textProps.y);
      }
    }

    bool touched(Point* touch) override {
      return area.check(touch->x, touch->y);
    }

    void addEventListener(EventCallback callback) override{
      area.addOnClickEvent(callback);
    }

    TouchButton area;
    IconProps iconProps;
    TextProps textProps;
  };
} // namespace ui