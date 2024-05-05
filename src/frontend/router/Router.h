#pragma once


#include "../views/home/HomeScreen.h"

/// @brief Routes view through given path names.
/// Much like in a html page, inspired by React.
class Router
{
    ui::BaseScreen* getRoute();

public:
    Router() = default;

    void
    loop();
};