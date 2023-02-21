#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

#include "entry_list.hpp"
#include "file_vector_entry_list.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)

class Menu {
public:
    Menu() {
	options_ = std::make_unique<FileVectorEntryList>(
		FileVectorEntryList(L"hello", L"/home/wenzhou/Entries/files/hello.txt"));

	initscr(); // Start ncurses mode
	noecho(); // Don't echo keystrokes
	cbreak(); // Disable line buffering

	raw();
	keypad(stdscr, TRUE);
    }

    ~Menu() {
	endwin();
    }

    void DrawEntryList() {
	    int rows, cols;
	    getmaxyx(stdscr, rows, cols);
	    
	    // Calculate the number of visible rows (subtract 2 for the title and input box)
	    if (rows < 2) { return; }
	    size_t visible_rows = rows - 2;

	    size_t selected_index = options_->GetSelectedIndex();

	    // Calculate the range of visible options
	    size_t start_option = visible_rows > selected_index ? 0: std::max<size_t>(0, selected_index - visible_rows + 1);
	    size_t end_option = std::min(options_->SearchedSize(), start_option + visible_rows);

	    // Print the title row
	    //mvprintw(0, 0, "%ls", options_->GetTitle().c_str());
	    mvprintw(0, 0, "%zu %zu %zu", end_option, start_option, options_->GetSelectedIndex());

	    // Print input row
	    mvprintw(1, 0, "%-*ls", cols, options_->GetInputText().c_str());

	    // Print the visible menu options
	    for (size_t i = start_option; i < end_option; i++) {
		if (i == selected_index) {
		    attron(A_REVERSE);
		}

		mvprintw(i - start_option + 2, 0, "%-*ls", cols, options_->GetEntryAtIndex(i)->GetName().c_str());

		attroff(A_REVERSE);
	    }

	    for (size_t i = end_option; i < visible_rows; i++) {
		mvprintw(i - start_option + 2, 0, "%-*s", cols, "");
	    }

	    // Move the cursor to the input row
	    move(1, cur_x_);
    }

    std::wstring Run() {
	options_->Search();

	do {
	    DrawEntryList();
	} while (GetInput_());

	return options_->GetSelectedEntry()->GetString();
    }

private:
    bool GetInput_() {
	wint_t choice;
	get_wch(&choice);
	switch (choice) {
	    case KEY_DOWN:
		options_->Down();
		break;
	    case KEY_UP:
		options_->Up();
		break;
	    case CTRL_MASK('A'):
		options_->AddEntry();
		break;
	    case CTRL_MASK('S'):
		options_->ToggleSearch();
		break;
	    case CTRL_MASK('R'):
		options_->RemoveEntry();
		break;
		/*
	    case CTRL_MASK('I'):
		options_->InsertEntry(input_text_);
		break;
		*/
	    case CTRL_MASK('U'):
		options_->UpdateEntry();
		break;
	    case KEY_BACKSPACE:
	    case 127:  // Some systems use 127 instead of KEY_BACKSPACE
		if (!options_->GetInputText().empty()) {
		    options_->SetInputText(options_->GetInputText().substr(0, options_->GetInputText().size() - 1));
		    cur_x_--;
		}

		options_->Search();

		break;
	    case KEY_ENTER:
	    case '\n':
		return false;
		break;
	    case '\t':
		options_->SetInputText(options_->GetSelectedEntry()->GetName());
		break;
	    default:
		if (iswprint(choice)) {
		    options_->SetInputText(options_->GetInputText() + (wchar_t)choice);
		    cur_x_++;
		    options_->Search();
		}
		break;
	}

	return true;
    }

    std::unique_ptr<EntryList> options_;

    int cur_x_ = 0;
};
