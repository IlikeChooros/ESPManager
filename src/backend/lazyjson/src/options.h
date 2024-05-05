#pragma once

// Used for debugging lazy json code, will print messages related to lazy json parsing:
// - when lazy_parser is destroyed
// - when lazy objects are created, copied, moved, destroyed
// - when lazy objects are being parsed
// - when lazy objects are being accessed
#define DEBUG_LAZY_JSON false

#if DEBUG_LAZY_JSON
#   include <Arduino.h>
#endif

