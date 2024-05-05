#include "paths.h"

ui::path ROUTER_PATH = ui::path::HOME;
ui::path PREV_PATH = ui::path::HOME;

void changeRoute(const ui::path& route){
    PREV_PATH = ui::path(ROUTER_PATH);
    ROUTER_PATH = ui::path(route);
}

const ui::path currentRoute() {
    return ROUTER_PATH;
}

const ui::path previousRoute() {
    return PREV_PATH;
}