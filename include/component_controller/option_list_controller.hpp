#pragma once

#include "component_controller/component_controller.hpp"
#include "component_controller/selected_option_position_controller.hpp"

class OptionListController: public ComponentController {
public:
    OptionListController(MenuData* menu_data, SelectedOptionPositionController& selected_option_position_controller): ComponentController(menu_data), selected_position_controller_(selected_option_position_controller) {}

    void Search() {
	if (!(menu_data_->Mode == MenuData::SEARCH)) { return; }
	if (!menu_data_->Option_List->Search(menu_data_->Input)) { return; }

	selected_position_controller_.Reset();
	menu_data_->Changed.Option_List = true;
    }

    void Add() {
	OptionList::ModifyStatus m_s = menu_data_->Option_List->Add(menu_data_->Input);

	if (m_s.BackendError) {
	    my::log.Error(2) << "Backend failed to add string" << std::endl;
	} else if (m_s.Modified) {
	    Search();
	    my::log.Info() << "Added string: " << menu_data_->Input << std::endl;
	} else {
	    my::log.Info() << "String already exists: " << menu_data_->Input << std::endl;
	}
    }

    void Remove() {
	std::wstring name = menu_data_->Option_List->NameAt(menu_data_->SelectedOptionPosition);
	OptionList::ModifyStatus m_s = menu_data_->Option_List->Remove(menu_data_->SelectedOptionPosition);

	if (m_s.BackendError) {
	    my::log.Error(2) << "Backend failed to remove string" << std::endl;
	} else if (m_s.Modified) {
	    my::log.Info() << "Removed string: " << name << std::endl;
	    Search();
	} else {
	    my::log.Info() << "Can't remove string, probably already exists: " << name << std::endl;
	}
    }

    void Update() {
	OptionList::ModifyStatus m_s;
	if (menu_data_->Editing == MenuData::NAME) {
	    m_s = menu_data_->Option_List->Update(menu_data_->SelectedOptionPosition, menu_data_->Input);
	} else {
	    m_s = menu_data_->Option_List->UpdateData(menu_data_->SelectedOptionPosition, menu_data_->Input);
	}

	if (m_s.BackendError) {
	    my::log.Error(2) << "Update backend error" << std::endl;
	} else if (m_s.Modified) {
	    Search();
	}
    }

    void Insert() {
	OptionList::ModifyStatus m_s = menu_data_->Option_List->Insert(menu_data_->SelectedOptionPosition, menu_data_->Input);

	if (m_s.BackendError) {
	    my::log.Error(2) << "Insertion backend error" << std::endl;
	} else if (m_s.Modified) {
	    Search();
	}
    }
protected:
    SelectedOptionPositionController& selected_position_controller_;
};
