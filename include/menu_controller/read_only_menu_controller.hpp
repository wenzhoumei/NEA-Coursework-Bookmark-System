#pragma once

#include "component_controller/title_controller.hpp"
#include "component_controller/selected_option_position_controller.hpp"
#include "component_controller/option_list_controller.hpp"
#include "component_controller/cursor_position_controller.hpp"
#include "component_controller/input_controller.hpp"

#include "menu_controller.hpp"

/**
 * @brief Class that edits MenuData class in a read only menu, therefore doesn't edit MenuData::Option_List_
 */
class ReadOnlyMenuController: public MenuController {
public:
    ReadOnlyMenuController(MenuData& menu_data);

protected:
    TitleController Title_;

    CursorPositionController Cursor_Position_;
    SelectedOptionPositionController Selected_Option_Position_;
    OptionListController Option_List_;
    InputController Input_;

    /**
     * @brief Sets title with [] brackets around letter that indicates mode
     *
     * In format: <location><action>
     * With location being fully defined location of file or directory or other of destination action
     */
    void SetTitle() override;

    /**
     * Keys that edit option list return a info message
     */
    PossibleExit ProcessPossibleExit_(const wchar_t& c) override;

    /**
     * Keys that edit option list return a info message
     */
    SpecialChar ProcessSpecialChars_(const wchar_t& c) override;

    /**
     * All characters that fall through case statements, decides if it is printable if so and prints them, otherwise ignores them
     */
    void ProcessDefaultChar_(const wchar_t& c) override;

    /**
     * @brief If current MenuData::Option_List_ is a bookmark list, toggle between data and name
     */
    void ToggleData_();

    /**
     * @brief Execute input directly, ignore selected text
     *
     * @return Exit code of program
     */
    int ExecuteInput_();

    /**
     * @brief Execute selected option if there is one, else ExecuteInput_
     *
     * @return Exit code of program
     */
    int ExecuteSelected_();
};
