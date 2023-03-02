#pragma once
#include "component_controller.hpp"

class CursorPositionController: public ComponentController {
public:
    CursorPositionController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}

    /**
     * Moves cursor to the right if possible
     */
    void Right();

    /**
     * Moves cursor to the left if possible
     */
    void Left();
};
