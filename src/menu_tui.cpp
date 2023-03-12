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
    delete menu_controller_;
}

void MenuTUI::Close() {
    menu_view_.Close();
    my::log.SetMenuTUI(nullptr);
    Parser::Instance().SetMenuController(nullptr);
}

MenuTUI::MenuTUI(OptionList* option_list)
    : menu_data_(option_list), menu_view_(menu_data_)
{
    my::log.SetMenuTUI(nullptr);

    if (!menu_data_.Option_List->Load()) {
	menu_controller_ = new ReadOnlyMenuController(menu_data_);
	my::log.Warning() << "Option list failed to load, opening read only" << std::endl;
    } else if (!menu_data_.Option_List->Editable()) {
	menu_controller_ = new ReadOnlyMenuController(menu_data_);
	my::log.Info() << "Loaded option list successfully" << std::endl;
    } else {
	menu_controller_ = new EditableMenuController(menu_data_);
	my::log.Info() << "Loaded option list successfully" << std::endl;
    }

    menu_data_.History.push({ menu_data_.Option_List->GetActionToHere(), menu_data_.Option_List->GetLocation() });

    my::log.SetMenuTUI(this);
    Parser::Instance().SetMenuController(menu_controller_);
}


MenuController* MenuTUI::GetMenuController() { return menu_controller_; }

int MenuTUI::Open() {
    menu_view_.Start();
    menu_view_.Display();

    menu_controller_->SetTitle();

    int exit_code;
    wint_t char_choice;
    do {
	menu_view_.Display();
	get_wch(&char_choice);
    } while ((exit_code = menu_controller_->ProcessChar(char_choice)) == ExitCode::DontExit);

    Close();
    return exit_code;
}
