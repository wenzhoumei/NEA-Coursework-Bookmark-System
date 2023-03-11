#pragma once
#include "menu_data.hpp"

class ComponentController {
public:
    ComponentController(MenuData& menu_data)
	: menu_data_(menu_data) {}
protected:
    MenuData& menu_data_;
};
