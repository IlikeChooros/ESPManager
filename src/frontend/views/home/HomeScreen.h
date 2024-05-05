#pragma once

#include "../base/BaseScreen.h"
#include "../../components/IconElement.h"

namespace ui
{
  class HomeScreen : public BaseScreen
  {
    public:
    HomeScreen(TFT_eSPI *tft, TouchScreen* ts);
  };
} // namespace ui