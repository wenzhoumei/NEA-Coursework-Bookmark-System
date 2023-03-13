#pragma once
#include "read_only_menu_controller.hpp"

/**
 * @brief Class that edits MenuData class and can edit MenuData::Option_List_
 */
class EditableMenuController: public ReadOnlyMenuController {
public:
    using ReadOnlyMenuController::ReadOnlyMenuController;

protected:
    /**
     * @brief Sets title with [] brackets around letter that indicates mode
     *
     * In format: <location><action>
     * With location being fully defined location of file or directory or other of destination action
     */
    void SetTitle() override;

    /**
     * @brief Escape key and enter key are overwritten to account for insert and search modes
     */
    PossibleExit ProcessPossibleExit_(const wchar_t& c) override;

    /**
     * @brief All keys that change option list are overwritten
     */
    SpecialChar ProcessSpecialChars_(const wchar_t& c) override;

    /**
     * @brief Changes mode and deals with any changes to menu data that comes with it
     */
    void SetMode_(enum MenuData::Mode m);
};
