#pragma once
#include "menu_data.hpp"
#include <ncurses.h>

class MenuView {
public:
    MenuView(MenuData* menu_data): menu_data_(menu_data) {}

    void Start() {
	initscr(); // Start ncurses mode
	noecho(); // Don't echo keystrokes
	cbreak(); // Disable line buffering
	set_escdelay(0);

	raw();
	keypad(stdscr, TRUE);
    }

    ~MenuView() {
	endwin();
    }

    void Display() {
	curs_set(0);

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	UpdateRowInformation(rows);

	if (Start_Option_ != Previous_Start_Option_) { menu_data_->Changed.Option_List = true; }

	if (menu_data_->Changed.Title == true) {
	    UpdateTitle(cols);
	}

	if (menu_data_->Changed.Input == true) {
	    UpdateInput(cols);
	}

	if (menu_data_->Changed.Option_List == true ) {
	    UpdateMenuOptions(rows, cols);
	}

	UpdateSelectedOption(cols);

	UpdateCursorPosition();
	menu_data_->Changed.Reset();

	if (menu_data_->Mode == MenuData::SEARCH) {
	    curs_set(1);
	} else {
	    curs_set(2);
	}
    }

    void UpdateTitle(int cols) {
	mvprintw(0, 0, "%-*ls", cols, menu_data_->Title.c_str());
    }

    void UpdateRowInformation(int rows) {
	enum MenuData::Mode mode = menu_data_->Mode;

	size_t menu_rows = rows - 2;
	size_t menu_total_size = menu_data_->Option_List->GetSearched().size();
	size_t selected_index = menu_data_->SelectedOptionPosition;

	if (mode == MenuData::INSERT) { menu_rows -= 1; }

	Previous_Start_Option_ = Start_Option_;

	Start_Option_ = selected_index > menu_rows - 1 ? selected_index - menu_rows + 1: 0;
	Num_Options_ = std::min(menu_total_size, Start_Option_ + menu_rows);
    }

    void UpdateMenuOptions(int rows, int cols) {
	if (rows < 2) { return; }

	enum MenuData::Mode mode = menu_data_->Mode;
	size_t selected_index = menu_data_->SelectedOptionPosition;

	if (mode == MenuData::INSERT) {
	    attron(A_NORMAL);

	    for (size_t i = Start_Option_; i < selected_index; i++) {
		mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, menu_data_->Option_List->NameAt(i).c_str());
	    }

	    for (size_t i = selected_index; i < Num_Options_; i++) {
		mvprintw(i - Start_Option_ + 2 + 1, 0, "%-*ls", cols, menu_data_->Option_List->NameAt(i).c_str());
	    }

	    // Fill out remaining empty rows
	    for (size_t i = Num_Options_ + 1; i < (size_t)rows; i++) {
		mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, L"");
	    }

	    attroff(A_NORMAL);
	} else {
	    attron(A_NORMAL);

	    for (size_t i = Start_Option_; i < Num_Options_; i++) {
		mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, menu_data_->Option_List->NameAt(i).c_str());
	    }

	    // Fill out remaining empty rows
	    for (size_t i = Num_Options_; i < (size_t)rows; i++) {
		mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, L"");
	    }

	    attroff(A_NORMAL);
	}
    }

    void UpdateInput(int cols) {
	enum MenuData::Mode mode = menu_data_->Mode;

	mvprintw(0, 0, "%zu", menu_data_->SelectedOptionPosition);
	// Print input row
	std::wstring input_text = menu_data_->Input;
	mvprintw(1, 0, "%-*ls", cols, input_text.c_str());

	if (mode == MenuData::EDIT || mode == MenuData::INSERT) {
	    mvprintw(menu_data_->SelectedOptionPosition - Start_Option_ + 2, 0, "%-*ls", cols, input_text.c_str());
	}
    }

    void UpdateSelectedOption(int cols) {
	static size_t previous_position = 0;
	size_t new_position = menu_data_->SelectedOptionPosition - Start_Option_ + 2;


	// If no results, there is no selected
	if (menu_data_->Option_List->GetSearched().size() == 0 && menu_data_->Mode == MenuData::SEARCH) {
	    mvprintw(menu_data_->SelectedOptionPosition - Start_Option_ + 2, 0, "%-*s", cols, "");
	    mvchgat(2, 0, cols, A_NORMAL, 0, NULL);
	    return;
	}

	enum MenuData::Mode mode = menu_data_->Mode;
	enum MenuData::Editing editing = menu_data_->Editing;

	if (mode == MenuData::INSERT || mode == MenuData::EDIT) {
	    mvprintw(new_position, 0, "%-*ls", cols, menu_data_->Input.c_str());
	} else if (mode == MenuData::SEARCH) {
	    if (editing == MenuData::NAME) {
		mvprintw(new_position, 0, "%-*ls", cols, menu_data_->SelectedName().c_str());
	    } else {
		mvprintw(new_position, 0, "%-*ls", cols, menu_data_->SelectedData().c_str());
	    }
	}

	// Reverse foreground and background of selected line
	mvchgat(previous_position, 0, cols, A_NORMAL, 0, NULL);
	mvchgat(new_position, 0, cols, A_REVERSE, 0, NULL);

	previous_position = new_position;
    }

    void UpdateCursorPosition() {
	move(1, menu_data_->Cursor_Position);
    }

private:
    size_t Previous_Start_Option_ = -1;
    size_t Start_Option_;
    size_t Num_Options_;

    MenuData* menu_data_;
};
