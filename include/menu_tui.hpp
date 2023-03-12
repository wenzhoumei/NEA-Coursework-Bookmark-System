#pragma once

#include "option_list/option_list.hpp"
#include "menu_data.hpp"
#include "menu_view.hpp"
#include "menu_controller/menu_controller.hpp"

// Handles display of MenuData
// Recieves events and execute appropriate Controller function
class MenuTUI {
    MenuData menu_data_;
    MenuView menu_view_;

    MenuController* menu_controller_;
public:
    MenuTUI(OptionList* option_list);
    ~MenuTUI();

    int Open();
    void Close();

    MenuController* GetMenuController();
};
