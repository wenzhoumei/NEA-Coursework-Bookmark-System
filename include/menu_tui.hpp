#pragma once

#include "option_list/option_list.hpp"
#include "menu_data.hpp"
#include "menu_view.hpp"
#include "menu_controller/menu_controller.hpp"

/**
 * @brief Encapsulates all components of MVC menu and handles top level functions of a menu
 *
 * Used to create and close a menu
 * Handles initializing and view, data and control components and closing view
 */
class MenuTUI {
public:
    MenuTUI(OptionList* option_list);
    ~MenuTUI();

    /**
     * @brief Initializes and starts display of menu
     *
     * Determines level of permissions menu has and assigns appropriate child class of MenuController
     * @return Exit code
     */
    int Open();

    /**
     * @brief Makes sure ncurses endwin is called so that terminal will function normally after program
     *
     * Determines level of permissions menu has and assigns appropriate child class of MenuController
     * @return Exit code of executed option string
     */
    void Close();

    /**
     * @brief Gets menu controller
     *
     * Needed for different classes to change menu
     *
     * @return The menu controller
     */
    MenuController* GetMenuController();
private:
    MenuData Menu_Data_; ///< Contains all information about menu
    MenuView Menu_View_; ///< Used to view menu using Menu_Data_
    MenuController* Menu_Controller_; ///< Processes keypress and updates Menu_Data_ accordingly
};
