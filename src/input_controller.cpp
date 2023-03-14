#include "component_controller/input_controller.hpp"
#include "component_controller/cursor_position_controller.hpp"
#include "component_controller/option_list_controller.hpp"

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

#include <cstring>
#include <iostream>
#include <algorithm>

InputController::InputController(MenuData* menu_data, CursorPositionController& cursor_position_controller, OptionListController& options_controller): ComponentController(menu_data), Cursor_Position_Controller_(cursor_position_controller), Options_Controller_(options_controller) {}

void InputController::AddChar(wchar_t c) {
    if (!iswprint(c)) return;

    Menu_Data_->Input.insert(Menu_Data_->Cursor_Position, 1, c);
    Menu_Data_->Changed.Input = true;

    Cursor_Position_Controller_.Right();

    if (Menu_Data_->Mode == MenuData::SEARCH) {
	Options_Controller_.Search();
    }
}

void InputController::Backspace() {
    if (Menu_Data_->Cursor_Position == 0) return;

    Cursor_Position_Controller_.Left();
    Menu_Data_->Input.erase(Menu_Data_->Cursor_Position, 1);
    Menu_Data_->Changed.Input = true;

    if (Menu_Data_->Mode == MenuData::SEARCH) {
	Options_Controller_.Search();
    }
}

void InputController::SetText(std::wstring input_text) { 
    Menu_Data_->Input = input_text;

    Cursor_Position_Controller_.MoveEnd();
    Menu_Data_->Changed.Input = true;

    if (Menu_Data_->Mode == MenuData::SEARCH) {
	Options_Controller_.Search();
    }
}

void InputController::Copy() {
    if (Menu_Data_->IsSearchListEmpty()) { return; }
    FILE* pipe = popen("xclip -sel clip", "w");
    if (!pipe) {
        std::cerr << "Error: Failed to open pipe to xclip" << std::endl;
        return;
    }
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring text = Menu_Data_->SelectedMode == MenuData::NAME ? Menu_Data_->SelectedName(): Menu_Data_->SelectedData();
    std::string utf8_text = converter.to_bytes(text);
    std::fwrite(utf8_text.c_str(), sizeof(char), utf8_text.size(), pipe);
    pclose(pipe);
}

void InputController::Paste() {
    // Get the contents of the clipboard
    std::wstring clipboard_text = GetClipboardText_();

    // Do nothing if clipboard is empty
    if (clipboard_text.empty()) return;

    // Insert the clipboard text at the current cursor position
    Menu_Data_->Input.insert(Menu_Data_->Cursor_Position, clipboard_text);

    // Move the cursor to the end of the inserted text
    Menu_Data_->Cursor_Position += clipboard_text.size();

    // Mark input as changed
    Menu_Data_->Changed.Input = true;

    // If in search mode, update search options
    if (Menu_Data_->Mode == MenuData::SEARCH) {
	Options_Controller_.Search();
    }
}

/**
 * @brief Clears all text in input box
 */
void InputController::Clear() { 
    SetText(L"");
}

void InputController::SetTextToSelected() {
    if (Menu_Data_->Option_List->GetSearched().size() == 0) { return; }

    SetText(Menu_Data_->SelectedName());
}

void InputController::Autofill() {
    if (Menu_Data_->Option_List->GetSearched().size() == 0) { return; }

    // Check for prefix matches
    for (size_t i: Menu_Data_->Option_List->GetSearched()) {
	std::wstring str_check_match = Menu_Data_->Option_List->NameAt(i);
	if (str_check_match.substr(0, Menu_Data_->Input.size()) == Menu_Data_->Input) {
	    SetText(str_check_match);
	    return;
	}
    }
}

void InputController::SetTextToData() {
    if (Menu_Data_->Option_List->GetSearched().size() == 0) { return; }

    SetText(Menu_Data_->SelectedData());
}

std::wstring InputController::GetClipboardText_() {
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
