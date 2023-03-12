#pragma once
#include "component_controller.hpp"

class CursorPositionController: public ComponentController {
public:
    CursorPositionController(MenuData* menu_data);

    /**
     * Moves cursor to the right if possible
     */
    void Right();

    /**
     * Moves cursor to the left if possible
     */
    void Left();

    /**
     * Sets cursor position to end of input text
     */
    void MoveEnd();
};
