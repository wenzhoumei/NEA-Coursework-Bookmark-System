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

	if (menu_data_->Changed.Title == true) {
	    UpdateTitle(cols);
	}

	if (menu_data_->Changed.Input == true) {
	    UpdateInput(cols);
	}

	if (menu_data_->Changed.Option_List == true || start_option_ != previous_start_option_) {
	    UpdateMenuOptions(rows, cols);
	}


	UpdateSelectedOptionPosition(cols);
	UpdateCursorPosition();

	menu_data_->Changed.Reset();

	curs_set(1);
    }

    void UpdateTitle(int cols) {
	mvprintw(0, 0, "%*s", cols, menu_data_->Title.c_str());
    }

    void UpdateRowInformation(int rows) {
	enum MenuData::Mode mode = menu_data_->Mode;

	size_t menu_rows = rows - 2;
	size_t menu_total_size = menu_data_->Option_List->GetSearched().size();
	size_t selected_index = menu_data_->SelectedOptionPosition;

	if (mode == MenuData::INSERT) { menu_rows -= 1; }

	previous_start_option_ = start_option_;

	start_option_ = selected_index > menu_rows - 1 ? selected_index - menu_rows + 1: 0;
	num_options_ = std::min(menu_total_size, start_option_ + menu_rows);
    }

    void UpdateMenuOptions(int rows, int cols) {
	if (rows < 2) { return; }

	enum MenuData::Mode mode = menu_data_->Mode;
	size_t selected_index = menu_data_->SelectedOptionPosition;
	size_t menu_total_size = menu_data_->Option_List->GetSearched().size();

	if (mode == MenuData::INSERT) {
	    attron(A_NORMAL);

	    for (size_t i = start_option_; i < selected_index; i++) {
		mvprintw(i - start_option_ + 2, 0, "%-*s", cols, menu_data_->Option_List->At(i).c_str());
	    }

	    for (size_t i = selected_index; i < num_options_; i++) {
		mvprintw(i - start_option_ + 2, 0, "%-*s", cols, menu_data_->Option_List->At(i).c_str());
	    }

	    // Fill out remaining empty rows
	    for (size_t i = num_options_ + 1; i < (size_t)rows; i++) {
		mvprintw(i - start_option_ + 2, 0, "%-*s", cols, "");
	    }

	    attroff(A_NORMAL);
	} else {
	    attron(A_NORMAL);

	    for (size_t i = start_option_; i < num_options_; i++) {
		mvprintw(i - start_option_ + 2, 0, "%-*s", cols, menu_data_->Option_List->At(i).c_str());
	    }

	    // Fill out remaining empty rows
	    for (size_t i = num_options_; i < (size_t)rows; i++) {
		mvprintw(i - start_option_ + 2, 0, "%-*s", cols, "");
	    }

	    attroff(A_NORMAL);
	}

	// If no results, there is no selected
	if (menu_total_size == 0 && mode == MenuData::SEARCH) {
	    mvchgat(2, 0, cols, A_NORMAL, 0, NULL);
	}
    }

    void UpdateInput(int cols) {
	enum MenuData::Mode mode = menu_data_->Mode;

	// Print input row
	std::string input_text = menu_data_->Input;
	mvprintw(1, 0, "%-*s", cols, input_text.c_str());

	if (mode == MenuData::EDIT || mode == MenuData::INSERT) {
	    mvprintw(menu_data_->SelectedOptionPosition, 0, "%-*s", cols, input_text.c_str());
	}
    }

    void UpdateSelectedOptionPosition(int cols) {
	static int previous_position = -1;

	int new_position = menu_data_->SelectedOptionPosition - start_option_ + 2;

	if (previous_position != new_position) {
	    // Reverse foreground and background of selected line
	    mvchgat(new_position, 0, cols, A_NORMAL, 0, NULL);
	    mvchgat(new_position, 0, cols, A_REVERSE, 0, NULL);
	}

	previous_position = menu_data_->SelectedOptionPosition;
    }

    void UpdateCursorPosition() {
	static int previous_position = -1;

	int new_position = menu_data_->Cursor_Position;

	if (previous_position != new_position) {
	    move(1, new_position);
	}
    }

private:
    size_t previous_start_option_ = -1;
    size_t start_option_;
    size_t num_options_;

    MenuData* menu_data_;
};
