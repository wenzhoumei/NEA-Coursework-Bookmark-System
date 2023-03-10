#pragma once

#include "title_controller.hpp"
#include "selected_option_position_controller.hpp"
#include "option_list_controller.hpp"
#include "cursor_position_controller.hpp"
#include "input_controller.hpp"

#include "menu_controller.hpp"

class ReadOnlyMenuController: public MenuController {
public:
    using MenuController::MenuController;

protected:
    TitleController Title_ = TitleController(Menu_Data_);

    CursorPositionController Cursor_Position_ = CursorPositionController(Menu_Data_);
    SelectedOptionPositionController Selected_Option_Position_ = SelectedOptionPositionController(Menu_Data_);
    OptionListController Option_List_ = OptionListController(Menu_Data_, Selected_Option_Position_);
    InputController Input_ = InputController(Menu_Data_, Cursor_Position_, Option_List_);


    PossibleExit ProcessPossibleExit_(const wchar_t& c) override;
    SpecialChar ProcessSpecialChars_(const wchar_t& c) override;
    void ProcessDefaultChar_(const wchar_t& c) override;

    void ToggleData_();

    int ExecuteInput_();
    int ExecuteSelected_();
};
