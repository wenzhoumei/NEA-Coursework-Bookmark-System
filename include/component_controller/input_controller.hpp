#pragma once
#include "component_controller.hpp"
#include "cursor_position_controller.hpp"
#include "option_list_controller.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <locale>
#include <codecvt>
#include <ncurses.h>

class InputController: public ComponentController {
public:
    InputController(MenuData* menu_data, CursorPositionController& cursor_position_controller, OptionListController& options_controller): ComponentController(menu_data), Cursor_Position_Controller_(cursor_position_controller), Options_Controller_(options_controller) {}


    /**
     * @brief Adds character in cursor position
     * @param c is character to add
     */
    void AddChar(wchar_t c) {
	if (!iswprint(c)) return;

	menu_data_->Input.insert(menu_data_->Cursor_Position, 1, c);
	menu_data_->Changed.Input = true;

	Cursor_Position_Controller_.Right();

	if (menu_data_->Mode == MenuData::SEARCH) {
	    Options_Controller_.Search();
	}
    }


    /**
     * @brief You have menu_data_->Cursor_Position as a member variable
     * update menu_data_->Input
     */
    void Backspace() {
	if (menu_data_->Cursor_Position == 0) return;

	Cursor_Position_Controller_.Left();
	menu_data_->Input.erase(menu_data_->Cursor_Position, 1);
	menu_data_->Changed.Input = true;

	if (menu_data_->Mode == MenuData::SEARCH) {
	    Options_Controller_.Search();
	}
    }


    /**
     * @brief Sets text in input box
     * @param input_text is text to set it as
     */
    void SetText(std::wstring input_text) { 
	menu_data_->Input = input_text;
	
	Cursor_Position_Controller_.MoveEnd();
	menu_data_->Changed.Input = true;

	if (menu_data_->Mode == MenuData::SEARCH) {
	    Options_Controller_.Search();
	}
    }

/**
 * @brief Paste the contents of the clipboard into the input field at the current cursor position
 */
void Paste() {
    // Get the contents of the clipboard
    std::wstring clipboard_text = GetClipboardText_();

    // Do nothing if clipboard is empty
    if (clipboard_text.empty()) return;

    // Insert the clipboard text at the current cursor position
    menu_data_->Input.insert(menu_data_->Cursor_Position, clipboard_text);

    // Move the cursor to the end of the inserted text
    menu_data_->Cursor_Position += clipboard_text.size();

    // Mark input as changed
    menu_data_->Changed.Input = true;

    // If in search mode, update search options
    if (menu_data_->Mode == MenuData::SEARCH) {
        Options_Controller_.Search();
    }
}

    /**
     * @brief Clears all text in input box
     */
    void Clear() { 
	SetText(L"");
    }

    void SetTextToSelected() {
	if (menu_data_->Option_List->GetSearched().size() == 0) { return; }

	SetText(menu_data_->SelectedName());
    }

    void SetTextToData() {
	if (menu_data_->Option_List->GetSearched().size() == 0) { return; }

	SetText(menu_data_->SelectedData());
    }

protected:
    CursorPositionController& Cursor_Position_Controller_;
    OptionListController& Options_Controller_;

/**
 * @brief Get the contents of the clipboard
 * @return The clipboard text as a string
 */
std::wstring GetClipboardText_() {
    FILE* pipe = popen("xclip -o -sel clip", "r");
    if (!pipe) {
        return L"";
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    char clipboard_text[1024];
    size_t n = fread(clipboard_text, sizeof(char), 1023, pipe);
    if (n > 0) {
        clipboard_text[n] = '\0';
    } else {
        clipboard_text[0] = '\0';
    }

    std::string clipboard_text_str = std::string(clipboard_text);
    std::replace(clipboard_text_str.begin(), clipboard_text_str.end(), '\r', ' ');
    std::replace(clipboard_text_str.begin(), clipboard_text_str.end(), '\n', ' ');

    pclose(pipe);

    return converter.from_bytes(clipboard_text_str);
}
};
