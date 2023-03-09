#include "menu_controller.hpp"
#include "parser.hpp"
#include "exit_code.hpp"

class KeyBinds {
public:
    KeyBinds(MenuController& menu_controller, MenuData& menu_data, MenuView& menu_view)
	: menu_controller_(menu_controller), menu_data_(menu_data), menu_view_(menu_view)
    {
    }

    virtual void KeyDown() { return; }
    virtual void KeyUp() { return; }
    virtual void KeyLeft() { return; }
    virtual void KeyRight() { return; }
    virtual void CtrlA() { return; }
    virtual void CtrlR() { return; }
    virtual void CtrlK() { return; }
    virtual void CtrlE() { return; }
    virtual void CtrlD() { return; }
    virtual void Backspace() { return; }
    virtual void OtherKey(const wchar_t& c) { return; }

    virtual int Escape() = 0;
    virtual int CtrlEnter() { return ExitCode::DontExit; }
    virtual int Enter() = 0;

protected:
    MenuController& menu_controller_;
    MenuData& menu_data_;
    MenuView& menu_view_;
};

class SuccessfulModeKeybinds: public KeyBinds {
public:
    using KeyBinds::KeyBinds;

    void KeyUp() override {
	menu_controller_.Selected_Option_Position->Up(Max(), menu_data_.SelectedOptionPosition);
	ChangedSelectedOption();
    }

    void KeyDown() override {
	menu_controller_.Selected_Option_Position->Down(Max(), menu_data_.SelectedOptionPosition);
	ChangedSelectedOption();
    }

    void KeyLeft() override {
	if (menu_data_.Cursor_Position == 0) { return; }

	menu_data_.Cursor_Position--;
    }

    void KeyRight() override {
	if (menu_data_.Cursor_Position == menu_data_.Input.size()) { return; }
	if (menu_data_.Cursor_Position > menu_data_.Input.size()) {
	    menu_data_.Input = menu_data_.Input.size();
	    return;
	}

	menu_data_.Cursor_Position++;
    }

    void OtherKey(const wchar_t& c) override {
	SetText_(menu_data_.Input + std::wstring(1, c));

	menu_data_.Cursor_Position++;
    }

    void Backspace() override {
	if (menu_data_.Input == L"") return;

	menu_data_.Cursor_Position--;
	SetText_(menu_data_.Input.substr(0, menu_data_.Input.size() - 1));
    }

    int Escape() override {
	return ExitCode::Success;
    }

protected:
    virtual size_t Max() {
	return menu_data_.Option_List->SearchedSize();
    }

    void Search_() {
	if (!menu_data_.Option_List->Search(menu_data_.Input)) { return; }

	menu_data_.SelectedOptionPosition = 0;
	menu_data_.Changed.Option_List = true;
	ChangedSelectedOption();
    }

    virtual void SetText_(const std::wstring& input_text) { 
	menu_data_.Input = input_text;
	menu_data_.Cursor_Position = input_text.size();
	menu_data_.Changed.Input = true;
    }

    virtual void ChangedSelectedOption() = 0;
};

class SearchModeKeyBinds: public SuccessfulModeKeybinds{
public:
    using SuccessfulModeKeybinds::SuccessfulModeKeybinds;

    void OtherKey(const wchar_t& c) override {
	SuccessfulModeKeybinds::OtherKey(c);
	Search_();
	return;
    }

    void Backspace() override {
	SuccessfulModeKeybinds::Backspace();
	Search_();
	return;
    }

    void CtrlA() override {
	if (menu_data_.Option_List->Editable) {
	    OptionList::ModifyStatus m_s = menu_data_.Option_List->Add(menu_data_.Input);

	    if (m_s.BackendError) {
		// TODO
	    } else if (m_s.Modified) {
		Search_();
	    }
	}
    }

    void CtrlR() override {
	if (menu_data_.Option_List->Editable) {
	    OptionList::ModifyStatus m_s = menu_data_.Option_List->Remove(menu_data_.SelectedOptionPosition);

	    if (m_s.BackendError) {
		// TODO
	    } else if (m_s.Modified) {
		Search_();
	    }
	}
    }

    void CtrlK() override {
    }

    void CtrlE() override {
    }

    void CtrlD() override {
	if (menu_data_.Editing == MenuData::DATA) { menu_data_.Editing = MenuData::NAME; }
	else if (menu_data_.Editing == MenuData::NAME) { menu_data_.Editing = MenuData::DATA; }
    }

    int Escape() override {
	return ExitCode::Success;
    }

    int CtrlEnter() override {
	return Parser::Instance().Execute(menu_data_.Option_List->GetAction(), menu_data_.Input);
    }

    int Enter() override {
	std::wstring chosen_input;
	if (menu_data_.IsSearchListEmpty()) {
	    chosen_input = menu_data_.Input;
	} else {
	    chosen_input = menu_data_.SelectedData();
	}

	return Parser::Instance().Execute(menu_data_.Option_List->GetAction(), chosen_input);
    }
private:
    void ChangedSelectedOption() override {
	if (menu_data_.Editing == MenuData::DATA) {
	    menu_data_.Editing = MenuData::NAME;
	}
    };

    void SetText_(const std::wstring& input_text) override { 
	SuccessfulModeKeybinds::SetText_(input_text);
	Search_();
    }
};

class InsertModeKeyBinds: public SuccessfulModeKeybinds {
public:
    using SuccessfulModeKeybinds::SuccessfulModeKeybinds;

    void KeyDown() override {
	SuccessfulModeKeybinds::KeyDown();
    }

    virtual void KeyUp() override {
	SuccessfulModeKeybinds::KeyUp();
	menu_data_.Input = menu_data_.SelectedName();
	menu_data_.Changed.Input = true;
    }

    int Escape() {
	return true;
    }

    int Enter() override {
	OptionList::ModifyStatus m_s;
	if (menu_data_.Editing == MenuData::NAME) {
	    m_s = menu_data_.Option_List->Update(menu_data_.SelectedOptionPosition, menu_data_.Input);
	} else {
	    m_s = menu_data_.Option_List->UpdateData(menu_data_.SelectedOptionPosition, menu_data_.Input);
	}

	if (m_s.BackendError) {
	    // TODO
	} else if (m_s.Modified) {
	    Search_();
	}

	return ExitCode::DontExit;
    }

private:
    size_t Max() override {
	return menu_data_.Option_List->SearchedSize() + 1;
    }

    void ChangedSelectedOption() override {
	menu_data_.Input = menu_data_.SelectedName();
	menu_data_.Changed.Input = true;
    };
};

class EditModeKeyBinds: public SuccessfulModeKeybinds{
public:
    using SuccessfulModeKeybinds::SuccessfulModeKeybinds;

    void ChangePos_(size_t new_position) {
	menu_data_.SelectedOptionPosition = new_position;

	if (menu_data_.Mode == MenuData::INSERT) {
	} else if (menu_data_.Mode == MenuData::EDIT) {
	}
    }

    int Escape() override {
	menu_controller_.keybind = Search_Option_Keybind;
	Selected_Option_Position->Reset();
	menu_data_.Input->SetText(L"");
	return true;
    }

    int Enter() override {
	OptionList::ModifyStatus m_s = menu_data_.Option_List->Insert(menu_data_.SelectedOptionPosition, menu_data_.Input);

	if (m_s.BackendError) {
	    // TODO
	} else if (m_s.Modified) {
	    Search_();
	}
    }
protected:
    void ChangedSelectedOption() override {
	menu_data_.Input = menu_data_.SelectedName();
	menu_data_.Changed.Option_List = true;
	menu_data_.Changed.Input = true;
    };
};
