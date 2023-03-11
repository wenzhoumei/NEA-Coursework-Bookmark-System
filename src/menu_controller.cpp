#include "menu_controller/menu_controller.hpp"
#include "exit_code.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
#include <ncurses.h>

MenuController::MenuController(MenuData& menu_data)
    : Menu_Data_(menu_data) {}

int MenuController::ProcessChar(const wchar_t& c) {
    PossibleExit p_e = ProcessPossibleExit_(c);
    if (p_e.Matched) { return p_e.ReturnCode; }
    if (ProcessSpecialChars_(c).Matched) { return ExitCode::DontExit; }

    ProcessDefaultChar_(c);
    return ExitCode::DontExit;
}

void MenuController::UpdateStatus() {
    Menu_Data_.Changed.Status_Log = true;
}
