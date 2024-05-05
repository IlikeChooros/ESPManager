#include "Router.h"

extern ErrorProvider errorProvider;

extern TFT_eSPI tft;
extern TouchScreen ts;

void Router::loop(){
    std::unique_ptr<ui::BaseScreen> screen(getRoute());
    try{
        screen->draw();
        screen->loop();
    }
    catch(const std::exception& e){
        Lock lock(getSemaphore(tasks::error));
        changeRoute(ui::path::ERROR);
        errorProvider.reason(e.what());
        errorProvider.code("src/router/Router.cpp");
        errorProvider.log();
    }
}

ui::BaseScreen* Router::getRoute(){
    using namespace ui;
    switch (currentRoute())
    {
    default:
        return new HomeScreen(&tft, &ts);
    }
}