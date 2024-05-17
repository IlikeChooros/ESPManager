#pragma once

#include <TFT_eSPI.h>
#include <array>

enum class fonts{
    detail,
    title,
    normal,
};

class FontData
{
  public:
  FontData();

  /**
   * @brief Configures the fonts, finds the fonts in the SPIFFS and loads them,
   *  if not found, it will use the default fonts
  */
  void config();

  bool useCustomFonts;

  static const char* NORMAL_FONT;
  static const char* DETAIL_FONT;
  static const char* TITLE_FONT;

  static const char* useFont(const fonts& font);
};


extern FontData fontData;