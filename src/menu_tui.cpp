#include "menu_tui.hpp"
#include "exit_code.hpp"
#include "read_only_menu_controller.hpp"
#include "editable_menu_controller.hpp"

MenuTUI::MenuTUI(std::unique_ptr<OptionList> option_list)
{
    std::wstring action = option_list->GetAction();
    std::wstring location = option_list->GetLocation();

    menu_data_ = std::make_unique<MenuData>(MenuData(location + action, std::move(option_list)));
}

int MenuTUI::Open() {
    menu_view_ = std::make_unique<MenuView>(MenuView(menu_data_.get()));

    if (!menu_data_->Option_List->Load()) {
	// TODO
	Log::Instance().Warning() << "Failed to load, opening read only menu";
	menu_controller_ = std::make_unique<ReadOnlyMenuController>(ReadOnlyMenuController(*menu_data_));
    } else if (!menu_data_->Option_List->Editable()) {
	Log::Instance().Debug() << "opening not editable";
	menu_controller_ = std::make_unique<ReadOnlyMenuController>(ReadOnlyMenuController(*menu_data_));
    } else {
	Log::Instance().Debug() << "opening editable";
	menu_controller_ = std::make_unique<EditableMenuController>(EditableMenuController(*menu_data_));
    }

    menu_view_->Start();
    menu_view_->Display();

    int exit_code;
    wint_t char_choice;
    do {
	menu_view_->Display();
	get_wch(&char_choice);
    } while ((exit_code = menu_controller_->ProcessChar(char_choice)) == ExitCode::DontExit);

    return exit_code;
}

MenuController* MenuTUI::GetController() {
    return menu_controller_.get();
}
