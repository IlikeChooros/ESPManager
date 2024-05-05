#include "src/frontend/router/Router.h"

extern TFT_eSPI tft;
Router router;

void setup(){
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
}

void loop(){
  router.loop();
}