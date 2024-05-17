#include "HomeScreen.h"

namespace ui
{
  HomeScreen::HomeScreen(TFT_eSPI *tft, TouchScreen *ts) : BaseScreen(tft, ts)
  {
    _size = 2;
    _base_items = new ScreenElement *[_size]{
      new IconElement(10, 10, tft, IconProps(new SettingsIcon(), 32, BASE_SCREEN_BG, DEFAULT_BTN_COLOR)),
      new TextElement(160, 10, tft, TextProps("Home", placement::middle, BASE_SCREEN_BG, FontData::useFont(fonts::normal))),
    };
  }
} // namespace ui