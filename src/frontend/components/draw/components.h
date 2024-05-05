#pragma once

#include <TFT_eSPI.h>

void drawCard(
    uint16_t x, uint16_t y,
    uint16_t w, uint16_t h,
    uint16_t color, uint16_t bg,
    TFT_eSPI *tft
);

void drawInputField(
    uint16_t x, uint16_t y,
    uint16_t w, uint16_t h,
    uint16_t color, uint16_t outline,
    uint16_t bg, TFT_eSPI *tft
);

void drawWiFi(
    uint16_t x, 
    uint16_t y, 
    uint8_t size, 
    uint8_t strength, 
    uint16_t bg,
    TFT_eSPI *tft
);

void drawLoadingState(
    uint16_t x, uint16_t y,
    uint16_t bg, uint16_t size,
    int16_t angle,
    TFT_eSPI *tft
);

void clearLoadingState(
    uint16_t x, uint16_t y,
    uint16_t bg, uint16_t size,
    int16_t angle,
    TFT_eSPI *tft
);

void drawLocker(
    int16_t x, int16_t y,
    int16_t size, uint16_t bg, 
    bool unLocked,
    TFT_eSPI *tft
);