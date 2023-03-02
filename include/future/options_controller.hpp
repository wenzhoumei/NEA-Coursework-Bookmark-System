#pragma once

#include "component_controller.hpp"
#include "selected_option_position_controller.hpp"

class OptionsController: public ComponentController {
public:
    OptionsController(MenuData& menu_data, StatusLogController& status_log_controller, SelectedOptionPositionController& selected_option_position_controller): ComponentController(menu_data, status_log_controller), selected_position_controller_(selected_option_position_controller) {}

    void Search();

protected:
    SelectedOptionPositionController& selected_position_controller_;
};
