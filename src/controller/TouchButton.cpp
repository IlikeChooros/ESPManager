#include "TouchButton.h"

void do_nothing_button(){
    return;
}

TouchButton::
TouchButton(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height
)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    (void)this->addOnClickEvent(do_nothing_button);
}

TouchButton& TouchButton::
addOnClickEvent(std::function<void(void)> on_press){
    this->on_press = on_press;
    return *this;
}

bool TouchButton::
check(
    int16_t x, 
    int16_t y
)
{
    if((x >= this->x) && (x <= this->x + width)){
        if ((y >= this->y) && (y <= this->y + height)){
            on_touch();
            this->on_press();
            return true;
        }
    }
    return false;
}

void TouchButton::on_touch(){
    return;
}

void TouchButton::draw(){
    return;
}