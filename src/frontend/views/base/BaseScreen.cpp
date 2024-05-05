#include "BaseScreen.h"

extern ErrorProvider errorProvider;

namespace ui
{
    BaseScreen::BaseScreen(TFT_eSPI *tft, TouchScreen *ts):
_tft(tft), _ts(ts), _exited(false), _size(0), _base_items(nullptr), _base_bg(BASE_SCREEN_BG) {}

 BaseScreen::~BaseScreen(){
    if (!_base_items){
        return;
    }
    iterate([](ScreenElement* elem){delete elem;});
    delete [] _base_items;
}

void  BaseScreen::loop(){
    Point point;
    while(!_exited){
        if (_ts->read(point)){
            iterate([&](ScreenElement* elem) {elem->eventListener(&point);});
        }
    }
}

void  BaseScreen::
route(const ui::path& route){
    this->_exited = true;
    changeRoute(route);
}

void BaseScreen::handleError(){
    TryLock lock(getSemaphore(tasks::error), 2);
    if (!lock){
        return;
    }
    if (errorProvider.caught()){
        route(path::ERROR);
    }
}
 
void BaseScreen::iterate(std::function<void(ScreenElement*)> call){
    if (!_base_items){
        return;
    }
    for (uint8_t i = 0 ; i < _size; i++){
        call(_base_items[i]);
    }
}

void  BaseScreen::
draw(){
    if (!_base_items){
        return;
    }
    _tft->fillScreen(_base_bg);
    iterate([](ScreenElement* elem){elem->draw();});
}

}