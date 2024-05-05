#include "ScreenElement.h"

namespace ui
{
    ScreenElement::ScreenElement(TFT_eSPI *tft) : 
    _tft(tft), _exited(false), _redraw(true), _mounted(true) {}

    ScreenElement::~ScreenElement(){
        iterate([](TouchButton* btn){delete btn;});
    }

    void ScreenElement::
    draw(){
        if (!(_buttons.empty() && _redraw)){
            return;
        }
        iterate([](TouchButton* btn){btn->draw();});
        _redraw = false;
    }

    bool ScreenElement::
    eventListener(Point *touch){
        bool touched = false;
        iterate([&touched, &touch](TouchButton* btn){touched = touched || btn->check(touch->x, touch->y);});
        return touched;
    }

    void ScreenElement::iterate(
        std::function<void(TouchButton*)> call
    ){
        for (auto it = _buttons.begin(); it != _buttons.end(); it++){
            call(*it);
        }
    }

    void ScreenElement::onUpdate(){
        return;
    }

    void ScreenElement::updateState(){
        if (!_redraw){
            return;
        }
        draw();
        _redraw = false;
    }

} // namespace ui
