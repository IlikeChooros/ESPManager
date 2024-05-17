#pragma once

#include "../../controller/TouchButton.h"
#include "../../controller/TouchScreen.h"

#include <list>

namespace ui
{
    typedef std::function<void(void)> EventCallback;

    class ScreenElement{
        public:
        ScreenElement(TFT_eSPI* tft = nullptr);

        virtual
        ~ScreenElement();

        /// @brief Drawing function, by default draws all _buttons
        virtual void
        draw();

        /// @brief Executes when the area of this component is touched
        /// @param touch pixel coordinates of screen touch
        /// @return true if touch this component was touched
        virtual bool
        touched(Point* touch);

        /// @brief Adds event listener to component
        virtual void
        addEventListener(EventCallback callb);

        /// @brief Updates visuals of component, should be called after 
        /// `setData()` (implemented in `StorageElement`).
        /// By default draw's itself 
        virtual void
        updateState();

        /// @brief Called after `setData()` in StorageElement, should be implemented by child
        virtual void
        onUpdate();

        virtual
        void iterate(std::function<void(TouchButton*)>);

        TFT_eSPI *_tft;
        bool _exited;
        bool _mounted;
        bool _redraw;
        
        std::list<TouchButton*> _buttons;
    };
}