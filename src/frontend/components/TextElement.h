#pragma once

#include "ScreenElement.h"
#include "../font/FontLoader.h"

namespace ui
{
  class TextElement: public ScreenElement
  {
    public:
    TextElement(
      int x, int y, 
      TFT_eSPI* tft, 
      const TextProps& props
    ): ScreenElement(tft), props(props) {
      this->props.x = x;
      this->props.y = y;
    }

    void draw() override{
      FontLoader<TFT_eSPI> loader(_tft, props);
      _tft->drawString(props.text, props.x, props.y);
    }

    bool eventListener(Point* touch) override {
      return false;
    }

    TextProps props;
  };
}