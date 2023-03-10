#pragma once

#include "component_controller.hpp"
#include "selected_option_position_controller.hpp"

class OptionListController: public ComponentController {
public:
    OptionListController(MenuData& menu_data, SelectedOptionPositionController& selected_option_position_controller): ComponentController(menu_data), selected_position_controller_(selected_option_position_controller) {}

    void Search() {
	if (!(menu_data_.Mode == MenuData::SEARCH)) { return; }
	if (!menu_data_.Option_List->Search(menu_data_.Input)) { return; }

	selected_position_controller_.Reset();
	menu_data_.Changed.Option_List = true;
    }

    void Add() {
	OptionList::ModifyStatus m_s = menu_data_.Option_List->Add(menu_data_.Input);

	if (m_s.BackendError) {
	    Log::Instance().Error(2) << "Backend error";
	    // TODO
	} else if (m_s.Modified) {
	    Search();
	}
    }

    void Remove() {
	OptionList::ModifyStatus m_s = menu_data_.Option_List->Remove(menu_data_.SelectedOptionPosition);

	if (m_s.BackendError) {
	    Log::Instance().Error(2) << "Backend error";
	    // TODO
	} else if (m_s.Modified) {
	    Search();
	}
    }

    void Update() {
	OptionList::ModifyStatus m_s;
	if (menu_data_.Editing == MenuData::NAME) {
	    m_s = menu_data_.Option_List->Update(menu_data_.SelectedOptionPosition, menu_data_.Input);
	} else {
	    m_s = menu_data_.Option_List->UpdateData(menu_data_.SelectedOptionPosition, menu_data_.Input);
	}

	if (m_s.BackendError) {
	    Log::Instance().Error(2) << "Backend error";
	    // TODO
	} else if (m_s.Modified) {
	    Search();
	}
    }

    void Insert() {
	OptionList::ModifyStatus m_s = menu_data_.Option_List->Insert(menu_data_.SelectedOptionPosition, menu_data_.Input);

	if (m_s.BackendError) {
	    Log::Instance().Error(2) << "Backend error";
	    // TODO
	} else if (m_s.Modified) {
	    Search();
	}
    }

protected:
    SelectedOptionPositionController& selected_position_controller_;
};
