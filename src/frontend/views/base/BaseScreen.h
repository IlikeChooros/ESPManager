#pragma once

#include "../../../backend/providers/ErrorProvider.h"
#include "../../components/include.h"
#include "../../router/paths.h"
#include "../../../backend/tasks/tasks.h"


constexpr uint8_t MENU_SPACING = 15;
constexpr uint16_t BASE_SCREEN_BG = 0x0042;
constexpr uint16_t BASE_FIRST_OPTION_Y = 65;
constexpr uint16_t BASE_OPTION_HEIGHT = 40;

namespace ui
{

/// @brief Base class for the screens (views)
class BaseScreen
{
public:
    BaseScreen(TFT_eSPI *tft, TouchScreen *ts);

    virtual
    ~BaseScreen();

    /// @brief Changes `ROUTER_PATH` to given path
    /// @param path
    void
    route(const ui::path&);

    virtual void
    loop();

    virtual
    void draw();

    virtual
    void iterate(std::function<void(ScreenElement*)>);

    /// @brief Displays error exception that occured on Core 0 (backend core),
    /// routes to ErrorScreen, if one was thrown.
    void 
    handleError();

    TFT_eSPI *_tft;
    TouchScreen *_ts;
    bool _exited;
    ScreenElement** _base_items;
    uint8_t _size;
    uint16_t _base_bg;
};

}
