#pragma once
#include "menu_data.hpp"

class MenuController {
protected:
    struct PossibleExit { bool Matched; int ReturnCode; };
    struct SpecialChar { bool Matched; };

public:
    MenuController(MenuData& menu_data);

    int ProcessChar(const wchar_t& c);

    virtual void SetTitle();

protected:
    virtual PossibleExit ProcessPossibleExit_(const wchar_t& c) = 0;
    virtual SpecialChar ProcessSpecialChars_(const wchar_t& c) = 0;
    virtual void ProcessDefaultChar_(const wchar_t& c) = 0;
    MenuData& Menu_Data_;
};

/*
class MenuController {
public:
    MenuController(MenuData& menu_data);

    // matched, return code
    virtual std::pair<bool, int> ProcessPossibleExit(const wchar_t& c) = 0;
    // matched
    virtual bool ProcessSpecialChars(const wchar_t& c) = 0;
    virtual void ProcessDefaultChar(const wchar_t& c) = 0;

    virtual int ProcessChar(const wchar_t& c) {
	if (IsPossibleExit(c).first) {
	    return ProcessPossibleExit(c);
	}

	ProcessSpecialChars(c);
	ProcessDefaultChar(c);

	return ExitCode::DontExit;
    }

    virtual void SetTitle() { Menu_Data_.Title = Menu_Data_.Option_List->GetAction() + Menu_Data_.Option_List->GetAction(); }

protected:
    MenuData& Menu_Data_;
};

class ReadOnlyMenuController: public MenuController {
public:
    ReadOnlyMenuController(MenuData& menu_data);

    int ProcessPossibleExit(const wchar_t& c) {
	switch (c) {
	    case CTRL_MASK(KEY_ENTER):
		return CtrlEnter();
		break;
	    case KEY_ENTER:
	    case '\n':
		return Enter();
		break;
	    case KEY_ESCAPE:
		return ExitCode::Success();
		break;
	    }
	}
    }

    int ProcessSpecialChars(const wchar_t& choice) = 0;
    void ProcessDefaultChar(const wchar_t& choice) = 0;

    int ProcessChar(const wchar_t& choice) override {
	switch (choice) {
	    case KEY_DOWN:
		Selected_Option_Position->Down();
		break;
	    case KEY_UP:
		Selected_Option_Position->Up();
		break;
	    case KEY_LEFT:
		Cursor_Position->Left();
		break;
	    case KEY_RIGHT:
		Cursor_Position->Right();
		break;
	    case KEY_BACKSPACE:
	    case 127:  // Some systems use 127 instead of KEY_BACKSPACE
		Input->PopChar();
		break;
	    case KEY_ESCAPE:
		if (Menu_Data_.Mode != MenuData::SEARCH) {
		    SetMode(MenuData::SEARCH);
		} else {
		    return 0;
		}
		break;
	    case CTRL_MASK(KEY_ENTER):
		return CtrlEnter();
		break;
	    case KEY_ENTER:
	    case '\n':
		return Enter();
		break;
	    case '\t':
		Input->SetTextToSelected();
		break;
	    default:
		Input->AddChar(choice);
		break;
	}

	return ExitCode::DontExit;
    }


protected:
    int ExecuteInput_() {
	if (Menu_Data_.Input == L"") { return ExitCode::DontExit; }
	return Parser::Instance().Execute(Menu_Data_.Option_List->GetAction(), Menu_Data_.Input);
    }

    int ExecuteSelected_() {
	    std::wstring chosen_input;
	    if (Menu_Data_.IsSearchListEmpty()) {
		chosen_input = Menu_Data_.Input;
	    } else {
		chosen_input = Menu_Data_.SelectedData();
	    }

	    if (Menu_Data_.Mode == MenuData::SEARCH) {

		return Parser::Instance().Execute(Menu_Data_.Option_List->GetAction(), chosen_input);
	    } else if (Menu_Data_.Mode == MenuData::EDIT) {
		Option_List->Update();
		SetMode(MenuData::SEARCH);
	    } else if (Menu_Data_.Mode == MenuData::INSERT) {
		Option_List->Insert();
		SetMode(MenuData::SEARCH);
	    }

	    return ExitCode::DontExit;
    }

    TitleController Title;

    CursorPositionController Cursor_Position;
    SelectedOptionPositionController Selected_Option_Position;
    OptionListController Option_List;
    InputController Input;
};
*/

/*
class DefaultMenuController: public MenuController {
public:
    DefaultMenuController(MenuData& menu_data)
	: MenuController(menu_data) {}

    virtual int ProcessChar(const wchar_t& choice);

private:
    void SetMode(enum MenuData::Mode m);
    void ToggleData();
    int Enter();
    int CtrlEnter();

    TitleController Title;
    CursorPositionController Cursor_Position;
    SelectedOptionPositionController Selected_Option_Position;
    OptionListController Option_List;
};

class InputOnlyMenuController: public MenuController {
public:
    InputOnlyMenuController(MenuData& menu_data)
	: MenuController(menu_data) {}

    int ProcessChar(const wchar_t& choice) override;
};
*/

