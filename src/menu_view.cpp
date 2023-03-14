#include "menu_view.hpp"
#include <ncurses.h>

#include "log.hpp"

MenuView::MenuView(MenuData& menu_data): Menu_Data_(menu_data) {}

void MenuView::Initialize() {
    initscr(); // Start ncurses mode
    noecho(); // Don't echo keystrokes
    cbreak(); // Disable line buffering
    set_escdelay(0);
    start_color();

    init_pair(LogColor::Info, COLOR_BLACK, COLOR_WHITE);
    init_pair(LogColor::Warning, COLOR_BLACK, COLOR_YELLOW);
    init_pair(LogColor::Error, COLOR_BLACK, COLOR_RED);
    init_pair(LogColor::Debug, COLOR_BLACK, COLOR_GREEN);
    init_pair(LogColor::Normal, COLOR_WHITE, COLOR_BLACK);

    raw();
    keypad(stdscr, TRUE);
}

void MenuView::Close() {
    endwin();
}

MenuView::~MenuView() {
    Close();
}

void MenuView::Display() {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    if (rows != Previous_Rows_ || cols != Previous_Cols_) {
	Menu_Data_.Changed.SetAll();
    }

    if (rows < 3) { return; }

    UpdateRowInformation_(rows);

    if (Start_Option_ != Previous_Start_Option_) { Menu_Data_.Changed.Option_List = true; }

    if (Menu_Data_.Changed.Title) {
	UpdateTitle_(cols);
    }

    if (Menu_Data_.Changed.Input) {
	UpdateInput_(cols);
    }

    if (Menu_Data_.Changed.Option_List) {
	UpdateMenuOptions_(rows, cols);
    }

    UpdateSelectedOption_(cols);

    if (Menu_Data_.Changed.Status_Log) {
	UpdateStatusLog_(rows, cols);
    }

    UpdateCursorPosition_(cols);

    Menu_Data_.Changed.SetNone();

    Previous_Cols_ = cols;
    Previous_Rows_ = cols;

    if (Menu_Data_.Mode == MenuData::SEARCH) {
	curs_set(1);
    } else {
	curs_set(2);
    }
}

void MenuView::UpdateTitle_(int cols) {
    attron(A_BOLD);
    mvprintw(0, 0, "%-*ls", cols, Menu_Data_.Title.c_str());
    attroff(A_BOLD);
}

void MenuView::UpdateRowInformation_(int rows) {
    enum MenuData::Mode mode = Menu_Data_.Mode;

    size_t menu_rows = rows - 3;
    size_t menu_total_size = Menu_Data_.Option_List->GetSearched().size();
    size_t selected_index = Menu_Data_.Selected_Option_Position;

    if (mode == MenuData::INSERT) { menu_rows -= 1; }

    Previous_Start_Option_ = Start_Option_;
    Start_Option_ = selected_index > menu_rows - 1 ? selected_index - menu_rows + 1: 0;
    Num_Options_ = std::min(menu_total_size, Start_Option_ + menu_rows);
}

void MenuView::UpdateMenuOptions_(int rows, int cols) {
    enum MenuData::Mode mode = Menu_Data_.Mode;
    size_t selected_index = Menu_Data_.Selected_Option_Position;

    if (mode == MenuData::INSERT) {
	attron(A_NORMAL);

	for (size_t i = Start_Option_; i < selected_index; i++) {
	    mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, Menu_Data_.Option_List->NameAt(i).c_str());
	}

	for (size_t i = selected_index; i < Num_Options_; i++) {
	    mvprintw(i - Start_Option_ + 2 + 1, 0, "%-*ls", cols, Menu_Data_.Option_List->NameAt(i).c_str());
	}

	// Fill out remaining empty rows
	for (size_t i = Num_Options_ + 1; i < (size_t)rows - 3; i++) {
	    mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, L"");
	}

	attroff(A_NORMAL);
    } else {
	attron(A_NORMAL);

	for (size_t i = Start_Option_; i < Num_Options_; i++) {
	    mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, Menu_Data_.Option_List->NameAt(i).c_str());
	}

	// Fill out remaining empty rows
	for (size_t i = Num_Options_; i < (size_t)rows - 3; i++) {
	    mvprintw(i - Start_Option_ + 2, 0, "%-*ls", cols, L"");
	}

	attroff(A_NORMAL);
    }
}

void MenuView::UpdateInput_(int cols) {
    enum MenuData::Mode mode = Menu_Data_.Mode;

    std::wstring drawn_input_text;
    if ((int)Menu_Data_.Cursor_Position >= cols) {
	drawn_input_text = Menu_Data_.Input.substr(Menu_Data_.Cursor_Position - cols);
    } else {
	drawn_input_text = Menu_Data_.Input;
    }

    mvprintw(1, 0, "%-*ls", cols, drawn_input_text.c_str());

    if (mode == MenuData::EDIT || mode == MenuData::INSERT) {
	mvprintw(Menu_Data_.Selected_Option_Position - Start_Option_ + 2, 0, "%-*ls", cols, drawn_input_text.c_str());
    }
}

void MenuView::UpdateSelectedOption_(int cols) {

    size_t new_position = Menu_Data_.Selected_Option_Position - Start_Option_ + 2;


    // If no results, there is no selected
    if (Menu_Data_.Option_List->GetSearched().size() == 0 && Menu_Data_.Mode == MenuData::SEARCH) {
	mvprintw(Menu_Data_.Selected_Option_Position - Start_Option_ + 2, 0, "%-*s", cols, "");
	mvchgat(2, 0, cols, A_NORMAL, 0, NULL);
	return;
    }

    enum MenuData::Mode mode = Menu_Data_.Mode;
    enum MenuData::SelectedMode editing = Menu_Data_.SelectedMode;

    std::wstring new_name = Menu_Data_.SelectedName();

    int attr_selected = A_REVERSE;
    if (mode == MenuData::INSERT) {
	mvprintw(new_position, 0, "%-*ls", cols, Menu_Data_.Input.c_str());
    } else if (mode == MenuData::EDIT) {
	mvprintw(new_position, 0, "%-*ls", cols, Menu_Data_.Input.c_str());
	if (editing == MenuData::DATA) { attr_selected |= A_BOLD; } // Show data as bold 
    } else if (mode == MenuData::SEARCH) {
	if (editing == MenuData::NAME) {
	    if (!Menu_Data_.Changed.Option_List) { (mvprintw(Previous_Selected_Position_, 0, "%-*ls", cols, Previous_Selected_Name_.c_str())); }
	    mvprintw(new_position, 0, "%-*ls", cols, new_name.c_str());
	} else {
	    mvprintw(new_position, 0, "%-*ls", cols, Menu_Data_.SelectedData().c_str());
	    attr_selected |= A_BOLD; // Show data as bold
	}
    }

    // Reverse foreground and background of selected line
    mvchgat(new_position, 0, cols, attr_selected, 0, NULL);

    Previous_Selected_Position_ = new_position;
    Previous_Selected_Name_ = new_name;
}

void MenuView::UpdateCursorPosition_(int cols) {
    int drawn_cur_pos;
    if ((int)Menu_Data_.Cursor_Position >= cols) {
	drawn_cur_pos = cols - 1;
    } else {
	drawn_cur_pos = Menu_Data_.Cursor_Position;
    }

    move(1, drawn_cur_pos);
}

void MenuView::UpdateStatusLog_(int rows, int cols) {
    attron(COLOR_PAIR(my::log.GetCurrentColorForMenu()));
    mvprintw(rows - 1, 0, "%-*ls", cols, my::log.Peek().c_str());
    attron(COLOR_PAIR(LogColor::Normal));
}
