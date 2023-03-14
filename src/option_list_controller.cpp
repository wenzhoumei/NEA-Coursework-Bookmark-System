#include "component_controller/option_list_controller.hpp"
#include "log.hpp"

OptionListController::OptionListController(MenuData* menu_data, SelectedOptionPositionController& selected_option_position_controller): ComponentController(menu_data), selected_position_controller_(selected_option_position_controller) {}

void OptionListController::Search() {
    if (!(Menu_Data_->Mode == MenuData::SEARCH)) { return; }
    if (!Menu_Data_->Option_List->Search(Menu_Data_->Input)) { return; }

    selected_position_controller_.Reset();
    Menu_Data_->Changed.Option_List = true;
}

void OptionListController::Add() {
    OptionList::ModifyStatus m_s = Menu_Data_->Option_List->Add(Menu_Data_->Input);

    if (m_s.BackendError) {
	my::log.Error(ExitCode::WriteError) << "Backend failed to add string" << std::endl;
    } else if (m_s.Modified) {
	Search();
	my::log.Info() << "Added string: " << Menu_Data_->Input << std::endl;
    } else {
	my::log.Info() << "String already exists: " << Menu_Data_->Input << std::endl;
    }
}

void OptionListController::Remove() {
    std::wstring name = Menu_Data_->Option_List->NameAt(Menu_Data_->Selected_Option_Position);
    OptionList::ModifyStatus m_s = Menu_Data_->Option_List->Remove(Menu_Data_->Selected_Option_Position);

    if (m_s.BackendError) {
	my::log.Error(ExitCode::WriteError) << "Backend failed to remove string" << std::endl;
    } else if (m_s.Modified) {
	my::log.Info() << "Removed string: " << name << std::endl;
	Search();
    } else {
	my::log.Info() << "Can't remove string, probably doesn't exists: " << name << std::endl;
    }
}

void OptionListController::Update() {
    OptionList::ModifyStatus m_s;
    if (Menu_Data_->SelectedMode == MenuData::NAME) {
	m_s = Menu_Data_->Option_List->Update(Menu_Data_->Selected_Option_Position, Menu_Data_->Input);
    } else {
	m_s = Menu_Data_->Option_List->UpdateData(Menu_Data_->Selected_Option_Position, Menu_Data_->Input);
    }

    if (m_s.BackendError) {
	my::log.Error(ExitCode::WriteError) << "Update backend error" << std::endl;
    } else if (m_s.Modified) {
	Search();
    }
}

void OptionListController::Insert() {
    OptionList::ModifyStatus m_s = Menu_Data_->Option_List->Insert(Menu_Data_->Selected_Option_Position, Menu_Data_->Input);

    if (m_s.BackendError) {
	my::log.Error(ExitCode::WriteError) << "Insertion backend error" << std::endl;
    } else if (m_s.Modified) {
	Search();
    }
}
