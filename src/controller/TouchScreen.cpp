#include "TouchScreen.h"



uint16_t calData[5] = { 387, 3491, 258, 3495, 1 };

// Define the TFT_eSPI object
extern TFT_eSPI tft = TFT_eSPI();
extern TouchScreen ts = TouchScreen(&tft, calData);

void do_nothing(){
    return;
}

TouchScreen::TouchScreen(
    TFT_eSPI* tft, 
    uint16_t* data
)
{
    _tft = tft;
    tft->setTouch(data);
    _state = false;
    _lastState = false;
    _lastDebounceTime = 0;


    this->on_down(do_nothing);
    this->on_left(do_nothing);
    this->on_right(do_nothing);
    this->on_up(do_nothing);
    this->on_sleep(do_nothing);
    this->on_wakeup(do_nothing);
}

bool 
TouchScreen::read(Point& point)
{
    uint16_t x=0,y=0;
    bool state = _tft->getTouch(&x,&y);

    if (_state!=state){
        _state=state;

        // if this is an realse, then igonre
        if (!state){
            _lastState = state;
            return false;
        }
        point.x = x;
        point.y = y;
        return true;
    }
    _lastState = state;
    return false;
}

TouchPoint
TouchScreen::read_touch()
{
    uint16_t x=0,y=0;
    bool state = _tft->getTouch(&x,&y);
    TouchPoint touch = TouchPoint::null;

    if (_state!=state){
        _state=state;

        // if this is an realse, then igonre
        if (!state){
            _lastState = state;
            return touch;
        }

        // dividing screen to 4 triangles
        if (y > 0.75f*x){
            if ( y < -0.75f*x + 240){
                this->_on_left();
                touch = TouchPoint::left;
            }
            else{
                this->_on_down();
                touch = TouchPoint::down;
            }
        }
        else{
            if ( y < -0.75f*x + 240){
                this->_on_up();
                touch = TouchPoint::up;
            }
            else{
                this->_on_right();
                touch = TouchPoint::right;
            }
        }
    }
    _lastState = state;

    return touch;
}

TouchScreen&
TouchScreen::on_left(
    void(*left)(void)
)
{
    this->_on_left = left;
    return *this;
}

TouchScreen&
TouchScreen::on_right(
    void(*right)(void)
)
{
    this->_on_right = right;
    return *this;
}

TouchScreen&
TouchScreen::on_down(
    void(*down)(void)
)
{
    this->_on_down = down;
    return *this;
}

TouchScreen&
TouchScreen::on_up(
    void(*up)(void)
)
{
    this->_on_up = up;
    return *this;
}

TouchScreen&
TouchScreen::on_sleep(
    void(*sleep)()
)
{
    this->_on_sleep = sleep;
    return *this;
}

TouchScreen&
TouchScreen::on_wakeup(
    void(*wake_up)()
)
{
    this->_on_wakeup = wake_up;
    return *this;
}