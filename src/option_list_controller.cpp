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
	my::log.Write(L"WriteError: Backend failed to add string");
    } else if (m_s.Modified) {
	Search();
	my::log.Write(L"Info: Added string: " + Menu_Data_->Input);
    } else {
	my::log.Write(L"Info: String already exists: " + Menu_Data_->Input);
    }
}

void OptionListController::Remove() {
    std::wstring name;
    OptionList::ModifyStatus m_s;

    if (Menu_Data_->Selected_Option_Position < Menu_Data_->Option_List->SearchedSize() && Menu_Data_->Option_List->SearchedSize() != 0) {
	name = Menu_Data_->Option_List->NameAt(Menu_Data_->Selected_Option_Position);
	m_s = Menu_Data_->Option_List->Remove(Menu_Data_->Selected_Option_Position);
    } else {
	m_s = { false, false };
    }

    if (m_s.BackendError) {
	my::log.Write(L"WriteError: Backend failed to remove string");
	return;
    } else if (m_s.Modified) {
	my::log.Write(L"Info: Removed string: " + name);
	Search();
    } else {
	my::log.Write(L"Info: Can't remove string, probably doesn't exist");
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
	my::log.Write(L"WriteError: Update backend error");
    } else if (m_s.Modified) {
	Search();
    }
}

void OptionListController::Insert() {
    OptionList::ModifyStatus m_s = Menu_Data_->Option_List->Insert(Menu_Data_->Selected_Option_Position, Menu_Data_->Input);

    if (m_s.BackendError) {
	my::log.Write(L"WriteError: Insertion backend error");
    } else if (m_s.Modified) {
	Search();
    }
}
