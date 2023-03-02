#pragma once
#include "component_controller.hpp"

class SelectedOptionPositionController: public ComponentController {
public:
    SelectedOptionPositionController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}
};
