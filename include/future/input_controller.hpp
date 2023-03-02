#pragma once
#include "component_controller.hpp"
#include "cursor_position_controller.hpp"
#include "options_controller.hpp"

class InputController: public ComponentController {
public:
    InputController(MenuData& menu_data, StatusLogController& status_log_controller, CursorPositionController& cursor_position_controller, OptionsController& options_controller): ComponentController(menu_data, status_log_controller), cursor_position_controller_(cursor_position_controller), options_controller_(options_controller) {}

    /**
     * @brief Sets text in input box
     * @param text is text to set it as
     */
    void SetText(const std::string& text);

    /**
     * @brief Removes last character from input box if it is not empty
     */
    void Backspace();

    /**
     * @brief Adds character in cursor position
     * @param c is character to add
     */
    void AddChar(char c);

protected:
    CursorPositionController& cursor_position_controller_;
    OptionsController& options_controller_;
};
