#include "HomeScreen.h"

namespace ui
{
  HomeScreen::HomeScreen(TFT_eSPI *tft, TouchScreen *ts) : BaseScreen(tft, ts)
  {
    _size = 1;
    _base_items = new ScreenElement *[_size]{
      new IconElement(10, 10, tft, IconProps(new SettingsIcon(), 32, BASE_SCREEN_BG, DEFAULT_BTN_COLOR))
    };
  }
} // namespace ui