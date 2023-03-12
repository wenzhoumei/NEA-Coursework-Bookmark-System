#pragma once
#include "component_controller.hpp"
#include "cursor_position_controller.hpp"
#include "option_list_controller.hpp"

class InputController: public ComponentController {
public:
    InputController(MenuData* menu_data, CursorPositionController& cursor_position_controller, OptionListController& options_controller);

    /**
     * @brief Adds character in cursor position
     * @param c is character to add
     */
    void AddChar(wchar_t c);


    /**
     * @brief You have menu_data_->Cursor_Position as a member variable
     * update menu_data_->Input
     */
    void Backspace();


    /**
     * @brief Sets text in input box
     * @param input_text is text to set it as
     */
    void SetText(std::wstring input_text);

    /**
     * @brief Paste the contents of the clipboard into the input field at the current cursor position
     */
    void Paste();

    /**
     * @brief Clears all text in input box
     */
    void Clear();

    void SetTextToSelected();

    void Autofill();

    void SetTextToData();

private:
    CursorPositionController& Cursor_Position_Controller_;
    OptionListController& Options_Controller_;

    /**
     * @brief Get the contents of the clipboard
     * @return The clipboard text as a string
     */
    std::wstring GetClipboardText_();
};
