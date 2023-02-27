#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

#include "file_vector_entry_list.hpp"
#include "set_entry_list.hpp"
#include "directory_entry_list.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
class Menu {
public:
    Menu() {
	options_ = std::make_unique<FileVectorEntryList>(FileVectorEntryList(L"hello", "/home/wenzhou/Entries/files/hello.txt"));
	//options_ = std::make_unique<DirectorySetEntryList>(DirectorySetEntryList(L"hello", "/home/wenzhou/Downloads"));
	//options_ = std::make_unique<VectorEntryList>(VectorEntryList(L"hello"));
	//options_ = std::make_unique<SetEntryList>(SetEntryList(L"hello"));

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

	if (options_->Input_Changed() || options_->EntryList_Changed()) {
	    options_->SearchMenu_Search();
	}
	options_->Visibility_Update(rows, cols);

	size_t selected_index = options_->Selected_GetIndex();

	size_t start_option = options_->Visibility_StartOption();
	size_t num_options = options_->Visibility_NumOptions();

	EntryList::mode mode = options_->Mode_Get();

	// Print the title row
	//mvprintw(0, 0, "%-*zu", cols, selected_index);
	//mvprintw(0, 0, "%zu %zu %zu", num_options, start_option, selected_index);
	mvprintw(0, 0, "%zu %zu", selected_index, options_->SearchMenu_Size());
	if (options_->Title_Changed()) {
	    //mvprintw(0, 0, "%ls", options_->GetTitle().c_str());
	    //mvprintw(0, 0, "%zu %zu %zu", end_option, start_option, options_->GetSelectedIndex());
	    //mvprintw(0, 0, "%zu", options_->GetCursorPosition());
	    options_->Title_UpdateBackend();
	}
	
	if (mode == EntryList::INSERT) {
	    attron(A_NORMAL);

	    for (size_t i = start_option; i < selected_index; i++) {
		//mvprintw(0, 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
		mvprintw(options_->Visibility_TranslateIndexToRow(i), 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
	    }

	    for (size_t i = selected_index; i < num_options; i++) {
		//mvprintw(0, 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
		mvprintw(options_->Visibility_TranslateIndexToRow(i) + 1, 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
	    }

	    // Fill out remaining empty rows
	    for (size_t i = num_options + 1; i < (size_t)rows; i++) {
		mvprintw(options_->Visibility_TranslateIndexToRow(i), 0, "%-*s", cols, "");
	    }

	    attroff(A_NORMAL);
	} else if (options_->SearchMenu_NeedsDisplayUpdate()) {
	    attron(A_NORMAL);

	    for (size_t i = start_option; i < num_options; i++) {
		//mvprintw(0, 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
		mvprintw(options_->Visibility_TranslateIndexToRow(i), 0, "%-*ls", cols, options_->SearchMenu_Get(i)->GetName().c_str());
	    }

	    // Fill out remaining empty rows
	    for (size_t i = num_options; i < (size_t)rows; i++) {
		mvprintw(options_->Visibility_TranslateIndexToRow(i), 0, "%-*s", cols, "");
	    }

	    attroff(A_NORMAL);
	}

	// Print input row
	if (options_->Input_Changed()) {
	    std::wstring input_text = options_->Input_GetText();
	    mvprintw(1, 0, "%-*ls", cols, input_text.c_str());

	    if (mode == EntryList::EDIT || mode == EntryList::INSERT) {
		mvprintw(options_->Visibility_SelectedRow(), 0, "%-*ls", cols, input_text.c_str());
	    }
	}

	// Reverse foreground and background of selected line
	mvchgat(options_->Visibility_PreviousSelectedRow(), 0, cols, A_NORMAL, 0, NULL);
	mvchgat(options_->Visibility_SelectedRow(), 0, cols, A_REVERSE, 0, NULL);

	// If no results, there is no selected
	if (options_->SearchMenu_Size() == 0 && mode == EntryList::SEARCH) {
	    mvchgat(2, 0, cols, A_NORMAL, 0, NULL);
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

	return options_->EntryList_GetEntry()->GetString();
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
	    case CTRL_MASK('k'):
		options_->Mode_Set(EntryList::INSERT);
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
	    case KEY_ESCAPE:
		options_->Selected_Reset();
		options_->Mode_Set(EntryList::SEARCH);
		break;
	    case KEY_ENTER:
	    case '\n':
		if (options_->Mode_Get() == EntryList::SEARCH) {
		    return false;
		} else if (options_->Mode_Get() == EntryList::EDIT) {
		    options_->EntryList_UpdateEntry();
		} else if (options_->Mode_Get() == EntryList::INSERT) {
		    options_->EntryList_InsertEntry();
		}
		break;
	    case '\t':
		options_->Input_SetTextToSelected();
		break;
	    default:
		options_->Input_PrintableKey((wchar_t)choice);
		break;
	}

	return true;
    }

    std::unique_ptr<EntryList> options_;
};

// Handles display of MenuData
// Recieves events and execute appropriate Controller function
class MenuTUI {
public:
    MenuTUI(MenuOption menu_option) {
        menu_data_ = new MenuData();
        menu_controller_ = new MenuController(*menu_data_, menu_option);
    }

    ~MenuTUI(); // Delete menu controllers and data

    void Display(); // Display menu_data_

    std::wstring Input(); // Get character and handle with appropriate controller function

private:
    MenuData* menu_data_;

    MenuController* menu_controller_;
};
