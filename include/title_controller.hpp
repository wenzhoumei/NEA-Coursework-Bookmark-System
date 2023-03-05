#pragma once
#include "component_controller.hpp"

class TitleController: public ComponentController {
public:
    TitleController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}

    /**
     * Sets text of title
     */
    void SetText();
};
