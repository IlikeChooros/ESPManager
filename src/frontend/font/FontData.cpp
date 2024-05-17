#include "FontData.h"

FontData fontData = FontData();

// Static variables and methods

const char* FontData::DETAIL_FONT = "UbuntuLight18";
const char* FontData::NORMAL_FONT = "UbuntuLight26";
const char* FontData::TITLE_FONT = "Ubuntu35";

const char* FontData::useFont(const fonts& font){
    switch(font){
        case fonts::detail:
            return DETAIL_FONT;
        case fonts::title:
            return TITLE_FONT;
        default:
            return NORMAL_FONT;
    }
}

// Class methods

FontData::FontData(): useCustomFonts(false) {}

void FontData::config(){
  String FILE_EXTENTION = ".vlw";

  struct _FontFile{
    String file;
    bool found;
  };

  // List of fonts to compare
  std::array<_FontFile, 3> compare {
    _FontFile{TITLE_FONT + FILE_EXTENTION, false},
    _FontFile{DETAIL_FONT + FILE_EXTENTION, false},
    _FontFile{NORMAL_FONT + FILE_EXTENTION, false},
  };

  if(!SPIFFS.begin(true)){
    return;
  }

  fs::FS fs = SPIFFS;
  fs::File root = fs.open("/");

  useCustomFonts = true;

  while(root){
    fs::File file = root.openNextFile();
    if (!file){
      break;
    }

    // Check if the file is a font, (not a directory)
    if (!file.isDirectory()){
      String name = file.name();

      // Check if that is the font we are looking for
      for (int i = 0; i < compare.size(); i++){
        if (name == compare[i].file){
          compare[i].found = true;
          break;
        }
      }
    } else {
      // Open the directory
      root = fs.open(file.name());
    }
  }

  // Check if all fonts were found
  for (int i = 0; i < compare.size(); i++){
    if (!compare[i].found){
      useCustomFonts = false;
      return;
    }
  }
}