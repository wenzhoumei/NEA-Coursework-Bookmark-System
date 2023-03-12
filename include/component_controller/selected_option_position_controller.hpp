#pragma once
#include "component_controller.hpp"

class SelectedOptionPositionController: public ComponentController {
public:
    SelectedOptionPositionController(MenuData* menu_data);

    void Down();
    void Up();

    void Reset();

private:
    size_t Max_();
    void ChangePos(size_t new_position);
};
