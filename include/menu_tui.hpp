#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <memory>

#include "option_list.hpp"
#include "menu_data.hpp"
#include "menu_view.hpp"
#include "menu_controller.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27

// Handles display of MenuData
// Recieves events and execute appropriate Controller function
class MenuTUI {
    std::unique_ptr<MenuData> menu_data_;
    std::unique_ptr<MenuController> menu_controller_;
    std::unique_ptr<MenuView> menu_view_;

public:
    MenuTUI(std::unique_ptr<OptionList> option_list, const std::string& action, const std::string& data)
    {
        menu_data_ = std::make_unique<MenuData>(MenuData(data + action, action, std::move(option_list)));
        menu_view_ = std::make_unique<MenuView>(MenuView(menu_data_.get()));
        menu_controller_ = std::make_unique<MenuController>(MenuController(*menu_data_));
    }

    void Start() {
	menu_view_->Start();
    }

    std::string Input() {
	while (GetChar_()) {
	    menu_view_->Display();
	}

	return menu_data_->Input;
    }

    MenuController* GetController() {
	return menu_controller_.get();
    }

private:
    bool GetChar_() {
	wint_t choice;
	get_wch(&choice);

	switch (choice) {
	    case KEY_DOWN:
		menu_controller_->Selected_Option_Position->Down();
		break;
	    case KEY_UP:
		menu_controller_->Selected_Option_Position->Up();
		break;
	    case KEY_LEFT:
		menu_controller_->Cursor_Position->Left();
		break;
	    case KEY_RIGHT:
		menu_controller_->Cursor_Position->Right();
		break;
	    case CTRL_MASK('a'):
		menu_controller_->Option_List->Add();
		break;
	    case CTRL_MASK('r'):
		menu_controller_->Option_List->Remove();
		break;
	    case CTRL_MASK('k'):
		menu_controller_->SetMode(MenuData::INSERT);
		break;
	    case CTRL_MASK('e'):
		menu_controller_->SetMode(MenuData::EDIT);
		break;
	    case CTRL_MASK('d'):
		// Change to data
		/*
		menu_controller_->GetSelectedEntry();
		*/
		break;
	    case KEY_BACKSPACE:
	    case 127:  // Some systems use 127 instead of KEY_BACKSPACE
		menu_controller_->Input->PopChar();
		break;
	    case KEY_ESCAPE:
		menu_controller_->SetMode(MenuData::SEARCH);
		break;
	    case KEY_ENTER:
	    case '\n':
		if (menu_data_->Mode == MenuData::SEARCH) {
		    return false;
		} else if (menu_data_->Mode == MenuData::EDIT) {
		    menu_controller_->Option_List->Update();
		} else if (menu_data_->Mode == MenuData::INSERT) {
		    menu_controller_->Option_List->Insert();
		}
		break;
	    case '\t':
		menu_controller_->Input->SetTextToSelected();
		break;
	    default:
		menu_controller_->Input->AddChar(choice);
		break;
	}

	return true;
    }
};
