#include "component_controller/title_controller.hpp"

TitleController::TitleController(MenuData* menu_data): ComponentController(menu_data) {}

void TitleController::SetText(const std::wstring& new_title) {
    Menu_Data_->Title = new_title;
    Menu_Data_->Changed.Title = true;
}
