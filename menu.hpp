#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

#include "entry_list.hpp"
#include "vector_file_entry_list.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)

class Menu {
public:
    Menu() {
	options_ = std::make_unique<VectorFileEntryList>(
		VectorFileEntryList("hello", "/home/wenzhou/Entries/files/hello.txt"));
	setlocale(LC_ALL, "");

	initscr(); // Start ncurses mode
	noecho(); // Don't echo keystrokes
	cbreak(); // Disable line buffering

	raw();
	keypad(stdscr, TRUE);
    }

    ~Menu() {
	endwin();
    }

    std::string Run() {
	options_->Search(input_text_);

	do {
	    int rows, cols;
	    getmaxyx(stdscr, rows, cols);
	    
	    // Calculate the number of visible rows (subtract 2 for the title and input box)
	    visible_rows_ = rows - 2;

	    // Calculate the range of visible options
	    int start_option = std::max(0, options_->GetSelected() - visible_rows_ + 1);
	    int end_option = std::min((int)options_->SearchedSize(), start_option + visible_rows_);

	    // Print the title row
	    mvprintw(0, 0, "%s", options_->GetTitle().c_str());

	    // Print input row
	    mvprintw(1, 0, "%-*s", cols, input_text_.c_str());

	    // Print the visible menu options
	    for (int i = start_option; i < end_option; i++) {
		if (i == options_->GetSelected()) {
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
	} while (GetInput_());

	return options_->AtIndex(options_->GetSelected())->GetString();
    }

private:
    bool GetInput_() {
	int choice = getch();
	switch (choice) {
	    case KEY_DOWN:
		options_->Down();
		break;
	    case KEY_UP:
		options_->Up();
		break;
	    case CTRL_MASK('a'):
		options_->AddEntry(input_text_);
		break;
	    case CTRL_MASK('s'):
		options_->ToggleSearch();
		break;
	    case CTRL_MASK('r'):
		options_->RemoveEntry();
		break;
	    case CTRL_MASK('i'):
		options_->InsertEntry(input_text_);
		break;
	    case CTRL_MASK('u'):
		options_->UpdateEntry(input_text_);
		break;
	    case KEY_BACKSPACE:
	    case 127:  // Some systems use 127 instead of KEY_BACKSPACE
		if (!input_text_.empty()) {
		    input_text_.pop_back();
		    cur_x_--;
		}

		options_->Search(input_text_);

		break;
	    case KEY_ENTER:
	    case '\n':
		return false;
		break;
	    default:
		if (isprint(choice)) {
		    input_text_ += choice;
		    cur_x_++;
		    options_->Search(input_text_);
		}
		break;
	}

	return true;
    }

    std::string input_text_ = "";
    std::unique_ptr<EntryList> options_;

    int visible_rows_;

    int cur_x_ = 0;
};
