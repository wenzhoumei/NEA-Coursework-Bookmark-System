#pragma once
#include "read_only_menu_controller.hpp"

class EditableMenuController: public ReadOnlyMenuController {
public:
    using ReadOnlyMenuController::ReadOnlyMenuController;

protected:
    PossibleExit ProcessPossibleExit_(const wchar_t& c) override;
    SpecialChar ProcessSpecialChars_(const wchar_t& c) override;

    void ToggleData_();
    void SetMode_(enum MenuData::Mode m);

    int ExecuteInput_();
    int ExecuteSelected_();
};
