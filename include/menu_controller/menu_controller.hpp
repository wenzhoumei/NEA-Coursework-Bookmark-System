#pragma once
#include "menu_data.hpp"

class MenuController {
protected:
    struct PossibleExit { bool Matched; int ReturnCode; };
    struct SpecialChar { bool Matched; };

public:
    MenuController(MenuData& menu_data);
    int ProcessChar(const wchar_t& c);

    virtual void SetTitle() = 0;

    void UpdateStatus();
protected:
    virtual PossibleExit ProcessPossibleExit_(const wchar_t& c) = 0;
    virtual SpecialChar ProcessSpecialChars_(const wchar_t& c) = 0;
    virtual void ProcessDefaultChar_(const wchar_t& c) = 0;
    MenuData& Menu_Data_;
};
