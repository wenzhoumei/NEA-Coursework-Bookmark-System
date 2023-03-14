#include "component_controller/cursor_position_controller.hpp"

CursorPositionController::CursorPositionController(MenuData* menu_data): ComponentController(menu_data) { }

void CursorPositionController::Right() {
    if (Menu_Data_->Cursor_Position == Menu_Data_->Input.size()) { return; }

    Menu_Data_->Cursor_Position++;
}

void CursorPositionController::Left() {
    if (Menu_Data_->Cursor_Position <= 0) { return; }

    Menu_Data_->Cursor_Position--;
}

void CursorPositionController::MoveEnd() {
    Menu_Data_->Cursor_Position = Menu_Data_->Input.size();
}
