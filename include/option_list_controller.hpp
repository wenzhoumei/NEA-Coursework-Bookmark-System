#pragma once

#include "component_controller.hpp"
#include "selected_option_position_controller.hpp"

class OptionListController: public ComponentController {
public:
    OptionListController(MenuData& menu_data, StatusLogController& status_log_controller, SelectedOptionPositionController& selected_option_position_controller): ComponentController(menu_data, status_log_controller), selected_position_controller_(selected_option_position_controller) {}

    void Search() {
	if (!menu_data_.Option_List->Search(menu_data_.Input)) { return; }
	selected_position_controller_.Reset();

	menu_data_.Changed.Option_List = true;
    }

    void Add() {
	if (!menu_data_.Option_List->Add(menu_data_.Input)) { return; }

	menu_data_.Changed.Option_List = true;
    }
    void Remove() {
	if (!menu_data_.Option_List->Remove(menu_data_.SelectedOptionPosition)) { return; }

	menu_data_.Changed.Option_List = true;
    }

    void Update() {
	if (!menu_data_.Option_List->Update(menu_data_.SelectedOptionPosition, menu_data_.Input)) { return; }

	menu_data_.Changed.Option_List = true;
    }

    void Insert() {
	if (!menu_data_.Option_List->Insert(menu_data_.SelectedOptionPosition, menu_data_.Input)) { return; }

	menu_data_.Changed.Option_List = true;
    }

protected:
    SelectedOptionPositionController& selected_position_controller_;
};
