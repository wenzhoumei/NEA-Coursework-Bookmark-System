#include "menu_controller.hpp"
#include "menu_view.hpp"

class KeyBinds {
public:
    KeyBinds(MenuController& menu_controller, MenuData& menu_data, MenuView& menu_view);

    virtual void Initialize();

    virtual void KeyDown();
    virtual void KeyUp();
    virtual void KeyLeft();
    virtual void KeyRight();
    virtual void CtrlA();
    virtual void CtrlR();
    virtual void CtrlK();
    virtual void CtrlE();
    virtual void CtrlD();
    virtual void Backspace();
    virtual void OtherKey(const wchar_t& c);

    virtual int Escape() = 0;
    virtual int CtrlEnter();
    virtual int Enter() = 0;

    virtual MenuView* GetMenuView() = 0;
protected:
    virtual void SetKeyBind_(KeyBinds* keybind);

    MenuController& menu_controller_;
    MenuData& menu_data_;
    MenuView& menu_view_;
    
    std::unique_ptr<MenuView> ViewMode;
};

class DefaultModesKeyBinds: public KeyBinds {
public:
    using KeyBinds::KeyBinds;

    void Initialize() override;

    void KeyUp() override;
    void KeyDown() override;
    void KeyLeft() override;
    void KeyRight() override;
    void OtherKey(const wchar_t& c) override;
    void Backspace() override;
    void CtrlD() override;
    int Escape() override;

protected:
    virtual size_t Max();
    void Search_();
    virtual void SetText_(const std::wstring& input_text);
    virtual void ChangedSelectedOption_() = 0;
};

class SearchModeKeyBinds: public DefaultModesKeyBinds{
public:
    using DefaultModesKeyBinds::DefaultModesKeyBinds;

    void OtherKey(const wchar_t& c) override;

    void Backspace() override;

    void CtrlA() override;
    void CtrlR() override;
    void CtrlK() override;
    void CtrlE() override;

    int Escape() override;

    int CtrlEnter() override;

    int Enter() override;
private:
    void ChangedSelectedOption_() override;
    void SetText_(const std::wstring& input_text) override;
};

class InsertModeKeyBinds: public DefaultModesKeyBinds {
public:
    using DefaultModesKeyBinds::DefaultModesKeyBinds;

    int Escape() override;
    int Enter() override;
    void CtrlD() override;

private:
    size_t Max() override;
    void ChangedSelectedOption_() override;
};

class EditModeKeyBinds: public DefaultModesKeyBinds {
public:
    using DefaultModesKeyBinds::DefaultModesKeyBinds;

    int Escape() override;
    int Enter() override;
protected:
    void ChangedSelectedOption_() override;
};
