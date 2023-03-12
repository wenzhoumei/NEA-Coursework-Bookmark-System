#pragma once

#include "component_controller/component_controller.hpp"
#include "component_controller/selected_option_position_controller.hpp"

class OptionListController: public ComponentController {
public:
    OptionListController(MenuData* menu_data, SelectedOptionPositionController& selected_option_position_controller);

    void Search();
    void Add();
    void Remove();
    void Update();
    void Insert();

protected:
    SelectedOptionPositionController& selected_position_controller_;
};
