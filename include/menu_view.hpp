#pragma once
#include "menu_data.hpp"

class MenuView {
public:
    MenuView(MenuData& menu_data);

    void Start();

    void Close();

    ~MenuView();

    void Display();

    void UpdateTitle(int cols);

    void UpdateRowInformation(int rows);

    void UpdateMenuOptions(int rows, int cols);

    void UpdateInput(int cols);

    void UpdateSelectedOption(int cols);

    void UpdateCursorPosition(int cols);

    void UpdateStatusLog(int rows, int cols);

private:
    size_t Previous_Start_Option_ = -1;
    size_t Start_Option_;
    size_t Num_Options_;

    MenuData& menu_data_;

    size_t Previous_Selected_Position_ = 0;
    std::wstring Previous_Selected_Name_ = L"";
};
