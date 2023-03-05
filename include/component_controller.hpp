#pragma once
#include "menu_data.hpp"

class StatusLogController;
class ComponentController {
public:
    ComponentController(MenuData& menu_data, StatusLogController& status_log_controller)
	: menu_data_(menu_data), status_log_controller_(status_log_controller) {}
protected:
    MenuData& menu_data_;
    StatusLogController& status_log_controller_;
};
