#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

#include "option_list/option_list.hpp"
#include "menu_data.hpp"
#include "menu_view.hpp"
#include "menu_controller/menu_controller.hpp"

// Handles display of MenuData
// Recieves events and execute appropriate Controller function
class MenuTUI {
    std::unique_ptr<MenuData> menu_data_;
    std::unique_ptr<MenuController> menu_controller_;
    std::unique_ptr<MenuView> menu_view_;

public:
    MenuTUI(std::unique_ptr<OptionList> option_list);
    int Open();

    MenuController* GetMenuController() { return menu_controller_.get(); }

    void Close();

private:
    int GetChar_();
};
