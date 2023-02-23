#pragma once

#include <memory>
#include <string>
#include "entry.hpp"

class EntryList {
public:
    enum mode { SEARCH, EDIT };
    void Mode_Set(enum mode m) { Mode_mode_ = m; }
    enum mode Mode_Get() { return Mode_mode_; }

protected:
    void Mode_ResetToSearch() { 
	if (Mode_mode_ != SEARCH) {
	    Mode_mode_ = SEARCH;
	    SearchMenu_Update();
	}
    }

    enum mode Mode_mode_;
    bool Mode_decay_to_search_;

public:
    std::wstring Title_Get() { return Title_text_; }
    virtual bool Title_UpdateBackend() { return true; }
    bool Title_NeedsUpdate() { return Title_needs_update_; }

protected:
    bool Title_needs_update_;
    std::wstring Title_text_;

public:
    // If not enough, return false
    bool Visibility_Update(size_t visible_rows, size_t visible_cols) {
	if (visible_rows < 2) { return false; }
	if (visible_cols < 20) { return false; }

	size_t visible_menu_rows = visible_rows - 2;
	Visibility_num_cols = visible_cols;

	Visibility_start_option_ = Selected_GetIndex() > visible_menu_rows ? Selected_GetIndex() - visible_menu_rows + 1: 0;
	Visibility_end_option_ = std::min(SearchMenu_Size(), Visibility_start_option_ + visible_menu_rows);
	return true;
    }

    size_t Visibility_StartOption() { return Visibility_start_option_; }
    size_t Visibility_EndOption() { return Visibility_end_option_; }
    size_t Visibility_TranslateIndexToRow(size_t i) { return i - Visibility_start_option_ + 2; }

protected:
    size_t Visibility_start_option_;
    size_t Visibility_end_option_;
    size_t Visibility_num_cols;

public:
    void Selected_Down() { 
	if (SearchMenu_Size() == 0) { return; }
	if (Selected_selected_ >= SearchMenu_Size() - 1) { return; }

	Selected_SetPos_(Selected_selected_ + 1);
    }

    void Selected_Up() { 
	if (Selected_selected_ <= 0) { return; }
	
	Selected_SetPos_(Selected_selected_ - 1);
    }

    size_t Selected_GetIndex() { return Selected_selected_; }
    size_t Selected_GetPreviousIndex() { return Selected_previous_position_; }
    Entry* Selected_GetEntry() { return SearchMenu_Get(Selected_selected_); }
protected:
    void Selected_ReduceIfAboveMaximum_() {
	if (Selected_selected_ > SearchMenu_Size() - 1) { Selected_selected_ = SearchMenu_Size() - 1; }
    }

    void Selected_SetPos_(size_t new_sel_pos) {
	if (Mode_mode_ == EDIT) {
	    Input_SetTextToSelected();
	}
	Selected_previous_position_ = Selected_selected_;
	Selected_selected_ = new_sel_pos;
    }

    void Selected_Reset_() { 
	if (SearchMenu_Size() == 0) { return; }
	
	Selected_selected_ = 0;
    }

    size_t Selected_previous_position_;
    size_t Selected_selected_;

public:
    void Cursor_Left() { 
	if (Cursor_cur_x_ <= 0) { return; }

	Cursor_SetXPos_(Cursor_cur_x_ - 1);
    }

    void Cursor_Right() { 
	if (Cursor_cur_x_ >= Input_text_.size()) { return; }
	
	Cursor_SetXPos_(Cursor_cur_x_ + 1);
    }

    size_t Cursor_GetXPosition() { return Cursor_cur_x_; }

protected:
    size_t Cursor_cur_x_;
    size_t Cursor_previous_position_;

    void Cursor_SetXPos_(size_t new_cur_pos) {
	Cursor_previous_position_ = Cursor_cur_x_;
	Cursor_cur_x_ = new_cur_pos;
    }

public:
    void Input_SetTextToSelected() { 
	Input_SetText(Selected_GetEntry()->GetDisplayString());
    }

    void Input_PrintableKey(const wchar_t &input_char) {
	if (!iswprint(input_char)) return;

	Input_text_.insert(Cursor_cur_x_, 1, input_char);
	Input_needs_update_ = true;

	Cursor_SetXPos_(Cursor_cur_x_ + 1);

	SearchMenu_Update();
    }

    void Input_Backspace() {
	if (Input_text_ == L"") return;

	Input_needs_update_ = true;

	Cursor_Left();
	Input_text_.pop_back();
	SearchMenu_Update();
    }

    std::wstring Input_GetText() { return Input_text_; }

    void Input_SetText(const std::wstring& input_text) { 
	Input_text_ = input_text;
	
	Cursor_SetXPos_(input_text.size());
	Input_needs_update_ = true;

	SearchMenu_Update();
    }

    bool Input_NeedsUpdate() { return Input_needs_update_; }

protected:
    bool Input_needs_update_;
    std::wstring Input_text_;

public:
    virtual bool Update_Initialize() {
	Selected_selected_ = 0;

	Title_needs_update_ = Input_needs_update_ = SearchMenu_needs_update_ = true;
	Mode_mode_ = SEARCH;

	Cursor_cur_x_ = 0;
	Cursor_previous_position_ = -1;
	Selected_previous_position_ = -1;

	return true;
    }

    virtual bool Update_EndLoop() {
	Title_needs_update_ = Input_needs_update_ = SearchMenu_needs_update_ = false;
	return true;
    }

    // Return false if any of these fail else return true
    virtual void EntryList_RemoveEntry() = 0;
    virtual void EntryList_AddEntry() = 0;
    virtual void EntryList_InsertEntry() = 0;
    virtual void EntryList_UpdateEntry() = 0;

    virtual bool EntryList_UpdateBackend() { return true; }

public:
    virtual size_t SearchMenu_Size() const = 0;
    virtual void SearchMenu_Update() = 0;
    virtual Entry* SearchMenu_Get(size_t i) = 0;

    bool SearchMenu_NeedsUpdate() { return SearchMenu_needs_update_; }
protected:
    bool SearchMenu_needs_update_;

public:
    virtual void Debug_PrintAllEntries() = 0;
    virtual void Debug_PrintSearchedEntries() = 0;
};
