#pragma once

#include <memory>

#include "ScreenElement.h"
#include "./icons/icons.h"
#include "../data/Props.h"

namespace ui
{
  class IconElement: public ScreenElement
  {
    public:
    IconElement(
      int x, int y, 
      TFT_eSPI* tft, 
      const IconProps& props
    ): ScreenElement(tft), props(props) {
      this->props.x = x;
      this->props.y = y;
    }

    void draw(){
      if(props.icon){
        props.icon->draw(props.x, props.y, props.size, props.color, props.bg);
      }
    }

    bool touched(Point* touch) override {
      return false;
    }

    IconProps props;
  };
} // namespace ui