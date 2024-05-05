#pragma once

#include <Arduino.h>

namespace ui
{
    enum class path {
        HOME,
        ERROR,
    };
}

void changeRoute(const ui::path& route);

const ui::path currentRoute();
const ui::path previousRoute();