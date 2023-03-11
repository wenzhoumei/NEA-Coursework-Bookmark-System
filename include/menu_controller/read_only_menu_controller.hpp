#pragma once

#include "component_controller/title_controller.hpp"
#include "component_controller/selected_option_position_controller.hpp"
#include "component_controller/option_list_controller.hpp"
#include "component_controller/cursor_position_controller.hpp"
#include "component_controller/input_controller.hpp"

#include "menu_controller.hpp"

class ReadOnlyMenuController: public MenuController {
public:
    ReadOnlyMenuController(MenuData& menu_data)
	: MenuController(menu_data),
	  Title_(menu_data),
	  Cursor_Position_(menu_data),
	  Selected_Option_Position_(menu_data),
	  Option_List_(menu_data, Selected_Option_Position_),
	  Input_(menu_data, Cursor_Position_, Option_List_)
    { }

protected:
    TitleController Title_;

    CursorPositionController Cursor_Position_;
    SelectedOptionPositionController Selected_Option_Position_;
    OptionListController Option_List_;
    InputController Input_;

    void SetTitle() override;

    PossibleExit ProcessPossibleExit_(const wchar_t& c) override;
    SpecialChar ProcessSpecialChars_(const wchar_t& c) override;
    void ProcessDefaultChar_(const wchar_t& c) override;

    void ToggleData_();

    int ExecuteInput_();
    int ExecuteSelected_();
};
