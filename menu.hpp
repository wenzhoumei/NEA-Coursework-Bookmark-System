#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

#include "entry_list.hpp"

class Menu {
public:
    Menu(const std::string title, std::unique_ptr<EntryList> options) : title_(title), options_(std::move(options)), sel_(0) {
	setlocale(LC_ALL, "");

	initscr(); // Start ncurses mode
	noecho(); // Don't echo keystrokes
	cbreak(); // Disable line buffering

	keypad(stdscr, TRUE);
	clear();
    }

    ~Menu() {
	endwin();
    }

    int run() {
	options_->Search(input_text_);

	do {
	    int rows, cols;
	    getmaxyx(stdscr, rows, cols);
	    
	    // Calculate the range of visible options
	    int start_option = std::max(0, sel_ - visible_rows_ + 1);
	    int end_option = std::min((int)options_->SearchedSize(), start_option + visible_rows_);

	    // Calculate the number of visible rows (subtract 2 for the title and input box)
	    visible_rows_ = rows - 2;

	    // Print the title row
	    mvprintw(0, 0, "%d", (int)options_->SearchedSize());

	    // Print input row
	    mvprintw(1, 0, "%-*s", cols, input_text_.c_str());

	    // Print the visible menu options
	    for (int i = start_option; i < end_option; i++) {
		if (i == sel_) {
		    attron(A_REVERSE);
		}

		mvprintw(i - start_option + 2, 0, "%-*s", cols, options_->AtIndex(i)->GetName().c_str());

		attroff(A_REVERSE);
	    }

	    for (int i = end_option; i < visible_rows_; i++) {
		mvprintw(i - start_option + 2, 0, "%-*s", cols, "");
	    }

	    // Move the cursor to the input row
	    move(1, cur_x_);
	} while (getInput());

	return sel_;
    }

    bool getInput() {
	int choice = getch();
	switch (choice) {
	    case KEY_DOWN:
		sel_++;
		if (sel_ >= options_->SearchedSize()) {
		    sel_ = options_->SearchedSize() - 1;
		}
		break;
	    case KEY_UP:
		sel_--;
		if (sel_ < 0) {
		    sel_ = 0;
		}
		break;
	    case KEY_BACKSPACE:
	    case 127:  // Some systems use 127 instead of KEY_BACKSPACE
		if (!input_text_.empty()) {
		    input_text_.pop_back();
		    cur_x_--;
		    options_->Search(input_text_);
		    mvprintw(1, cur_x_, " ");  // Overwrite deleted character with space
		}

		break;
	    case KEY_ENTER:
	    case '\n':
		return false;
		break;
	    default:
		if (isprint(choice)) {
		    input_text_ += choice;
		    mvprintw(1, 0, "%s", input_text_.c_str());
		    cur_x_++;
		    options_->Search(input_text_);
		}
		break;
	}

	return true;
    }

private:
    std::string title_;
    std::string input_text_ = "";
    std::unique_ptr<EntryList> options_;

    int visible_rows_;

    int cur_x_ = 0;
    int sel_ = 0;
};
