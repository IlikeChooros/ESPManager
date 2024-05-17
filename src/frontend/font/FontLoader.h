#pragma once

#include <type_traits>

#include "FontData.h"
#include "../data/Props.h"

/**
 * @brief A simple class to load fonts, when object is created, the font is loaded
 * and when the object is destroyed, the font is unloaded
 * @tparam TFTType - The type of the TFT object, either TFT_eSPI or TFT_eSprite
*/
template <typename TFTType = TFT_eSPI>
class FontLoader
{
public:
  typedef struct {
    int8_t size;
    int8_t font_index;
  } _DefaultFontProps;

  /**
   * @brief Construct a new Font Loader object
   * @param tft - The TFT object
  */
  FontLoader(TFTType* tft = nullptr);
  /**
   * @brief Construct a new Font Loader object
   * @param tft - The TFT object
   * @param props - The text properties
  */
  FontLoader(TFTType* tft, const TextProps& props);

  /**
   * @brief Destroy the Font Loader object and unload the font
  */
  ~FontLoader();

  /**
   * @brief Load the font and set the text properties to the TFT object
   * @param props - The text properties
  */
  void load(const TextProps& props);

  /**
   * @brief Unload the font from the TFT object (if loaded)
  */
  void unload();

private:
  TFTType* tft;
  _DefaultFontProps getDefaultProps(const TextProps& props);

  _DefaultFontProps detail;
  _DefaultFontProps normal;
  _DefaultFontProps title;
};

template <typename TFTType>
FontLoader<TFTType>::FontLoader(TFTType* tft): FontLoader(tft, TextProps()) {}

template <typename TFTType>
FontLoader<TFTType>::FontLoader(TFTType* tft, const TextProps& props)
{
  static_assert(std::is_base_of<TFT_eSPI, TFTType>(), "TFTType must be a subclass of TFT_eSPI");
  this->tft = tft;

  // Set default fonts {size, font index}
  detail = {1, 2};
  normal = {2, 2};
  title = {2, 4};

  load(props);
}

template <typename TFTType>
FontLoader<TFTType>::~FontLoader()
{
  unload();
}

template <typename TFTType>
typename FontLoader<TFTType>::_DefaultFontProps FontLoader<TFTType>::getDefaultProps(const TextProps& props)
{
  if (props.font == FontData::useFont(fonts::detail)){
    return detail;
  }
  else if (props.font == FontData::useFont(fonts::normal)){
    return normal;
  }
  else if (props.font == FontData::useFont(fonts::title)){
    return title;
  }
  return normal;
}

template <typename TFTType>
void FontLoader<TFTType>::load(const TextProps& props)
{
  if (!tft) {
    return;
  }

  TextProps defaultProps("Default", placement::start, TFT_BLACK, FontData::useFont(fonts::normal));

  if (props.font != ""){
    defaultProps = props;
  }

  tft->setTextColor(defaultProps.color, defaultProps.bg);

  if(fontData.useCustomFonts){
    tft->loadFont(defaultProps.font);
  } else {
    auto defaultFont = getDefaultProps(defaultProps);
    tft->setTextSize(defaultFont.size);
    tft->setTextFont(defaultFont.font_index);
  }

  switch(defaultProps.location){
    case placement::middle:
      tft->setTextDatum(TC_DATUM);
      break;
    case placement::end:
      tft->setTextDatum(TR_DATUM);
      break;
    default:
      tft->setTextDatum(TL_DATUM);
      break;
  }
}

template <typename TFTType>
void FontLoader<TFTType>::unload()
{
  if (!tft) {
    return;
  }
  tft->unloadFont();
}