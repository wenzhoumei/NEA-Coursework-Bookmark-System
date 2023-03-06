#pragma once
#include "component_controller.hpp"

class StatusLogController: public ComponentController {
public:
    StatusLogController(MenuData& menu_data): ComponentController(menu_data, *this) {}

    template<typename T> StatusLogController& operator<<(const T& value);
} ;
