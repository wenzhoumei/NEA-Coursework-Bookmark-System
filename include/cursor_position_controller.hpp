#pragma once

class CursorPositionController {
public:
    void Right(std::wstring input, _size_t& cur_pos) {
	if (menu_data_.Cursor_Position == menu_data_.Input.size()) { return; }
	if (menu_data_.Cursor_Position > menu_data_.Input.size()) {
	    menu_data_.Input = menu_data_.Input.size();
	    return;
	}

	menu_data_.Cursor_Position++;
    }

    /**
     * Moves cursor to the left if possible
     */
    void Left(size_t& cur_pos) {
	if (menu_data_.Cursor_Position <= 0) { return; }

	menu_data_.Cursor_Position--;
    }

    /**
     * Sets cursor position to end of input text
     */
    void MoveEnd(size_t& cur_pos) {
	menu_data_.Cursor_Position = menu_data_.Input.size();
    }
};
