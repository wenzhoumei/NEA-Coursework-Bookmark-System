#pragma once
#include "menu_data.hpp"

/**
 * @brief Handles rendering of a menu using MenuData class using ncurses
 */
class MenuView {
public:
    MenuView(MenuData& menu_data);
    ~MenuView();

    /**
     * @brief Initialization functions for display of menu
     */
    void Initialize();

    /**
     * @brief Resets terminal to its initial state
     */
    void Close();

    /**
     * @brief Renders menu according to contents of menu_data
     *
     * Gets number of rows and columns of the current window
     * Calls different functions that update different parts of the menu if they need updating
     */
    void Display();

private:
    size_t Previous_Cols_ = -1;
    size_t Previous_Rows_ = -1;

    /**
     * @brief Calculates and sets some information about which options correspond to which row
     *
     * Depends on Menu_Data_.Selected_Option_Position
     */
    void UpdateRowInformation_(int rows);
    size_t Previous_Start_Option_ = -1;
    size_t Start_Option_; ///> Index of search option that corresponds to first row on display
    size_t Num_Options_; ///> Number of rows corresponding to an option currently being displayed

    /**
     * @brief Updates title bar using Menu_Data_.Title at the top
     */
    void UpdateTitle_(int cols);

    /**
     * @brief Updates the search menu options
     */
    void UpdateMenuOptions_(int rows, int cols);

    /**
     * @brief Updates input box on the second row and potentially text of selected option depnding on the current Menu_Data_.Mode
     */
    void UpdateInput_(int cols);

    /**
     * @brief Updates style on selected option according to Menu_Data_.Mode, resets style of previous selected option
     */
    void UpdateSelectedOption_(int cols);

    size_t Previous_Selected_Position_ = 0;
    std::wstring Previous_Selected_Name_ = L"";

    /**
     * @brief Updates cursor position
     */
    void UpdateCursorPosition_(int cols);

    /**
     * @brief Updates status bar at the bottom
     */
    void UpdateStatusLog_(int rows, int cols);


    MenuData& Menu_Data_;
};
