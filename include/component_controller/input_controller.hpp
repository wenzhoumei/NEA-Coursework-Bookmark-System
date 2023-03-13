#pragma once
#include "component_controller.hpp"
#include "cursor_position_controller.hpp"
#include "option_list_controller.hpp"

/**
 * @brief Contains functions that correspond to actions that directly modify input text
 *
 * Each time input text is modified, option list is searched if on search mode
 */
class InputController: public ComponentController {
public:
    InputController(MenuData* menu_data, CursorPositionController& cursor_position_controller, OptionListController& options_controller);

    /**
     * @brief Adds character in cursor position and searches option list if on search mode
     * @param c Is the character to add
     */
    void AddChar(wchar_t c);


    /**
     * @brief Removes character at cursor position unless it is zero
     */
    void Backspace();


    /**
     * @brief Sets text in input box
     * @param input_text is text to set it as
     */
    void SetText(std::wstring input_text);

    /**
     * @brief Copies selected name or data to clipboard
     */
    void Copy();

    /**
     * @brief Paste the contents of the clipboard into the input field at the current cursor position
     */
    void Paste();

    /**
     * @brief Clears all text in input box
     */
    void Clear();

    /**
     * @brief Sets input text to selected
     */
    void SetTextToSelected();

    /**
     * @brief Fills with first name in the searched option list with matching prefix if there is one
     */
    void Autofill();

    /**
     * @brief Sets input to the selected data
     */
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
