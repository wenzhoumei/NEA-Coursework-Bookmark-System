#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

#include "vector_entry_list.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)

class Menu {
public:
    Menu() {
	//options_ = std::make_unique<FileVectorEntryList>(FileVectorEntryList(L"hello", "/home/wenzhou/Entries/files/hello.txt"));
	options_ = std::make_unique<VectorEntryList>(VectorEntryList(L"hello"));

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
	curs_set(0);
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	options_->Visibility_Update(rows, cols);

	size_t selected_index = options_->Selected_GetIndex();
	size_t previous_selected_index = options_->Selected_GetPreviousIndex();

	size_t start_option = options_->Visibility_StartOption();
	size_t end_option = options_->Visibility_EndOption();

	// Print the title row
	mvprintw(0, 0, "%zu %zu", start_option, end_option);
	//mvprintw(0, 0, "%-*zu", cols, selected_index);
	if (options_->Title_NeedsUpdate()) {
	    //mvprintw(0, 0, "%ls", options_->GetTitle().c_str());
	    //mvprintw(0, 0, "%zu %zu %zu", end_option, start_option, options_->GetSelectedIndex());
	    //mvprintw(0, 0, "%zu", options_->GetCursorPosition());
	    options_->Title_Update();
	}

	// Print input row
	if (options_->Input_NeedsUpdate()) {
	    std::wstring input_text = options_->Input_GetText();
	    mvprintw(1, 0, "%-*ls", cols, input_text.c_str());

	    if (options_->Mode_Get() == EntryList::EDIT) {
		mvprintw(options_->Visibility_TranslateIndexToRow(selected_index), 0, "%-*ls", cols, input_text.c_str());
	    }
	}

	// Print the visible menu options
	if (options_->SearchMenu_NeedsUpdate()) {
	    for (size_t i = start_option; i < end_option; i++) {
		//mvprintw(0, 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
		mvprintw(options_->Visibility_TranslateIndexToRow(i), 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
	    }

	    // Fill out empty remaining rows
	    for (size_t i = end_option; i < (size_t)rows; i++) {
		mvprintw(options_->Visibility_TranslateIndexToRow(i), 0, "%-*s", cols, "");
	    }

	    options_->SearchMenu_Update();
	}

	// Reverse foreground and background of selected line
	mvchgat(options_->Visibility_TranslateIndexToRow(previous_selected_index), 0, cols, A_NORMAL, 0, NULL);
	mvchgat(options_->Visibility_TranslateIndexToRow(selected_index), 0, cols, A_REVERSE, 0, NULL);

	if (options_->SearchMenu_Size() == 0) {
	    mvchgat(selected_index + 2, 0, cols, A_NORMAL, 0, NULL);
	}

	/* else if (options_->SearchedSize() == 1) {
	    mvchgat(selected_index + 2, 0, cols, A_REVERSE, 0, NULL);
	} */

	size_t curs_xpos = options_->Cursor_GetXPosition();
	move(1, curs_xpos);

	/*
	size_t curs_xpos = options_->GetCursorXPosition();
	if (curs_xpos != options_->Previous.CursorPosition) {
	    move(1, curs_xpos);
	    // Move the cursor to correct position on input row
	}
	*/

	curs_set(1);
    }

    std::wstring Run() {
	options_->Update_Initialize();

	do {
	    DrawEntryList();
	    options_->Update_EndLoop();
	} while (GetInput_());

	return options_->Selected_GetEntry()->GetString();
    }

private:
    bool GetInput_() {
	wint_t choice;
	get_wch(&choice);

	switch (choice) {
	    case KEY_DOWN:
		options_->Selected_Down();
		break;
	    case KEY_UP:
		options_->Selected_Up();
		break;
	    case KEY_LEFT:
		options_->Cursor_Left();
		break;
	    case KEY_RIGHT:
		options_->Cursor_Right();
		break;
	    case CTRL_MASK('a'):
		options_->EntryList_AddEntry();
		break;
	    case CTRL_MASK('r'):
		options_->EntryList_RemoveEntry();
		break;
	    case CTRL_MASK('p'):
		options_->EntryList_RemoveEntry();
		break;
	    case CTRL_MASK('u'):
		options_->EntryList_InsertEntry();
		break;
	    case CTRL_MASK('e'):
		options_->Mode_Set(EntryList::EDIT);
		break;
	    case CTRL_MASK('d'):
		// Change to data
		/*
		options_->GetSelectedEntry();
		*/
		break;
	    case KEY_BACKSPACE:
	    case 127:  // Some systems use 127 instead of KEY_BACKSPACE
		options_->Input_Backspace();
		break;
	    case KEY_ENTER:
	    case '\n':
		if (options_->Mode_Get() == EntryList::SEARCH) {
		    return false;
		} else {
		    options_->EntryList_UpdateEntry();
		}
		break;
	    case '\t':
		options_->Input_Tab();
		break;
	    default:
		options_->Input_PrintableKey((wchar_t)choice);
		break;
	}

	return true;
    }

    std::unique_ptr<EntryList> options_;
};
