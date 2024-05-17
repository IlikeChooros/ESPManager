#include "HomeScreen.h"

namespace ui
{
  HomeScreen::HomeScreen(TFT_eSPI *tft, TouchScreen *ts) : BaseScreen(tft, ts)
  {
    constexpr int widgetSize = 80, widgetMargin = 20;

    _size = 3;
    _base_items = new ScreenElement *[_size]{
      new TextElement(160, 10, tft, TextProps("Home", placement::middle, BASE_SCREEN_BG, FontData::useFont(fonts::title))),
      new WidgetElement(160 - widgetMargin - widgetSize, 60, widgetSize, tft, 
        IconProps(new SettingsIcon(),0, BASE_SCREEN_BG, DEFAULT_BTN_COLOR),
        TextProps("Settings")
      ),
      new WidgetElement(160 + widgetMargin, 60, widgetSize, tft, 
        IconProps(new WiFiIcon(),0, BASE_SCREEN_BG, DEFAULT_BTN_COLOR),
        TextProps("Scanner")
      ),
    };

    _base_items[1]->addEventListener([](){
      Serial.println("Settings");
    });
    _base_items[2]->addEventListener([](){
      Serial.println("Scanner");
    });
  }
} // namespace ui