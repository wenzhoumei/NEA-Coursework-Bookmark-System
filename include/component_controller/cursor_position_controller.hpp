#pragma once
#include "component_controller.hpp"

/**
 * @brief Handles events pertaining to cursor position
 */
class CursorPositionController: public ComponentController {
public:
    CursorPositionController(MenuData* menu_data);

    /**
     * @brief Moves cursor to the right if possible
     */
    void Right();

    /**
     * @brief Moves cursor to the left if possible
     */
    void Left();

    /**
     * @brief Sets cursor position to end of input text
     */
    void MoveEnd();
};
