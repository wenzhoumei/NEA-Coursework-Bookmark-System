#include "menu_tui.hpp"
#include "exit_code.hpp"
#include "parser.hpp"
#include "menu_controller/read_only_menu_controller.hpp"
#include "menu_controller/editable_menu_controller.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

MenuTUI::~MenuTUI() {
    Close();
    delete Menu_Controller_;
}

void MenuTUI::Close() {
    Menu_View_.Close();
    my::log.SetMenuTUI(nullptr);
    Parser::Instance().SetMenuController(nullptr);
}

MenuTUI::MenuTUI(OptionList* option_list)
    : Menu_Data_(option_list), Menu_View_(Menu_Data_)
{
    my::log.SetMenuTUI(nullptr);

    if (!Menu_Data_.Option_List->Load()) {
	Menu_Controller_ = new ReadOnlyMenuController(Menu_Data_);
	my::log.Warning() << "Option list failed to load, opening read only" << std::endl;
    } else if (!Menu_Data_.Option_List->Editable()) {
	Menu_Controller_ = new ReadOnlyMenuController(Menu_Data_);
	my::log.Info() << "Loaded option list successfully" << std::endl;
    } else {
	Menu_Controller_ = new EditableMenuController(Menu_Data_);
	my::log.Info() << "Loaded option list successfully" << std::endl;
    }

    Menu_Data_.History.push({ Menu_Data_.Option_List->GetActionToHere(), Menu_Data_.Option_List->GetLocation() });

    my::log.SetMenuTUI(this);
    Parser::Instance().SetMenuController(Menu_Controller_);
}


MenuController* MenuTUI::GetMenuController() { return Menu_Controller_; }

int MenuTUI::Open() {
    Menu_View_.Initialize();
    Menu_View_.Display();

    Menu_Controller_->SetTitle();

    int exit_code;
    wint_t char_choice;
    do {
	Menu_View_.Display();
	get_wch(&char_choice);
    } while ((exit_code = Menu_Controller_->ProcessChar(char_choice)) == ExitCode::DontExit);

    Close();
    return exit_code;
}
