#pragma once

#include <memory>
#include <string>
#include "entry.hpp"

class EntryList {
public:
    enum mode { SEARCH, EDIT, INSERT };
    void Mode_Set(enum mode m) {
	if (m == EDIT) {
	    Input_SetTextToSelected();
	} else if (m == INSERT) {
	    Input_SetText(L"");
	} else if (m == SEARCH) {
	    if (Mode_mode_ != SEARCH) {
		Mode_mode_ = SEARCH;
		Input_SetText(L"");
	    }
	}

	Mode_mode_ = m;
    }

    enum mode Mode_Get() { return Mode_mode_; }
protected:
    enum mode Mode_mode_;

public:
    std::wstring Title_Get() { return Title_text_; }
    virtual bool Title_UpdateBackend() { return true; }
    bool Title_Changed() { return Title_changed_; }

protected:
    bool Title_changed_;
    std::wstring Title_text_;

public:
    // If not enough, return false
    bool Visibility_Update(size_t visible_rows, size_t visible_cols) {
	if (visible_rows < 2) { return false; }
	if (visible_cols < 20) { return false; }

	size_t visible_menu_rows = visible_rows - 2;
	if (Mode_mode_ == INSERT) { visible_menu_rows--; }
	Visibility_num_cols = visible_cols;

	Visibility_previous_start_option_ = Visibility_start_option_;
	Visibility_start_option_ = Selected_GetIndex() > visible_menu_rows - 1 ? Selected_GetIndex() - visible_menu_rows + 1: 0;

	Visibility_previous_selected_row_ = Visibility_selected_row_;
	Visibility_selected_row_ = Visibility_TranslateIndexToRow(Selected_selected_);

	Visibility_end_option_ = std::min(SearchMenu_Size(), Visibility_start_option_ + visible_menu_rows);
	return true;
    }

    size_t Visibility_PreviousStartOption() { return Visibility_previous_start_option_; }
    size_t Visibility_StartOption() { return Visibility_start_option_; }
    bool Visibility_StartOptionChanged() { return Visibility_previous_start_option_ != Visibility_start_option_; }

    size_t Visibility_PreviousSelectedRow() { return Visibility_previous_selected_row_; }
    size_t Visibility_SelectedRow() { 
	return Visibility_selected_row_;
    }

    size_t Visibility_SelectedRowChanged() { return Visibility_selected_row_ != Visibility_previous_selected_row_; }

    size_t Visibility_NumOptions() {
	return Visibility_end_option_;
    }

    size_t Visibility_TranslateIndexToRow(size_t i) {
	return i - Visibility_start_option_ + 2;
    }

protected:
    size_t Visibility_start_option_;
    size_t Visibility_previous_start_option_;

    size_t Visibility_selected_row_;
    size_t Visibility_previous_selected_row_;

    size_t Visibility_end_option_;
    size_t Visibility_num_cols;
public:
    void Selected_Down() { 
	if (SearchMenu_Size() == 0) { return; }

	Selected_SetPos_(Selected_selected_ + 1);
    }

    void Selected_Up() { 
	if (Selected_selected_ <= 0) { return; }
	
	Selected_SetPos_(Selected_selected_ - 1);
    }

    size_t Selected_GetIndex() { return Selected_selected_; }
    Entry* Selected_GetEntry() { return SearchMenu_Get(Selected_selected_); }

protected:
    void Selected_ReduceIfAboveMaximum_() {
	if (Selected_selected_ > Selected_Max_()) { Selected_selected_ = Selected_Max_(); }
    }

    void Selected_SetPos_(size_t new_sel_pos) {
	if (Mode_mode_ == EDIT) {
	    Input_SetTextToSelected();
	}

	Selected_selected_ = new_sel_pos;
	Selected_ReduceIfAboveMaximum_();
    }

    size_t Selected_Max_() {
	if (Mode_mode_ == INSERT) {
	    return SearchMenu_Size();
	} else {
	    return SearchMenu_Size() - 1;
	}
    }

    void Selected_Reset_() { 
	if (SearchMenu_Size() == 0) { return; }
	
	Selected_selected_ = 0;
    }

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
    size_t Cursor_ChangedXPos() { return Cursor_cur_x_ == Cursor_previous_cur_x_; }

protected:
    size_t Cursor_cur_x_;
    size_t Cursor_previous_cur_x_;

    void Cursor_SetXPos_(size_t new_cur_pos) {
	Cursor_previous_cur_x_ = Cursor_cur_x_;
	Cursor_cur_x_ = new_cur_pos;
    }

public:
    void Input_SetTextToSelected() { 
	Input_SetText(Selected_GetEntry()->GetDisplayString());
    }

    void Input_PrintableKey(const wchar_t &input_char) {
	if (!iswprint(input_char)) return;

	Input_text_.insert(Cursor_cur_x_, 1, input_char);
	Input_changed_ = true;

	Cursor_SetXPos_(Cursor_cur_x_ + 1);
    }

    void Input_Backspace() {
	if (Input_text_ == L"") return;

	Input_changed_ = true;

	Cursor_Left();
	Input_text_.pop_back();
    }

    std::wstring Input_GetText() { return Input_text_; }

    void Input_SetText(const std::wstring& input_text) { 
	Input_text_ = input_text;
	
	Cursor_SetXPos_(input_text.size());
	Input_changed_ = true;
    }

    bool Input_Changed() { return Input_changed_; }

    Entry* Input_GetEntry() { 
	Input_unprocessed_input = std::make_unique<UnprocessedEntry>(UnprocessedEntry(Input_text_));
	return Input_unprocessed_input.get();
    }

protected:
    bool Input_changed_;
    std::wstring Input_text_;
    std::unique_ptr<Entry> Input_unprocessed_input;

public:
    virtual bool Update_Initialize() {
	Title_changed_ = Input_changed_ = EntryList_changed_ = true;
	Mode_mode_ = SEARCH;

	Selected_selected_ = Visibility_selected_row_ = Cursor_previous_cur_x_ = Cursor_cur_x_ = 0;
	Visibility_previous_selected_row_ = Cursor_previous_cur_x_ = -1;

	return true;
    }

    virtual bool Update_EndLoop() {
	Title_changed_ = Input_changed_ = EntryList_changed_ = false;
	return true;
    }

    // Return false if any of these fail else return true
    virtual void EntryList_RemoveEntry() = 0;
    virtual void EntryList_AddEntry() = 0;
    virtual void EntryList_InsertEntry() = 0;
    virtual void EntryList_UpdateEntry() = 0;

    virtual bool EntryList_UpdateBackend() { return true; }

    Entry* EntryList_GetEntry() { 
	if (SearchMenu_Size() == 0) {
	    return Input_GetEntry();
	} else {
	    return Selected_GetEntry();
	}
    }

    bool EntryList_Changed() { return EntryList_changed_; }

protected:
    bool EntryList_changed_;

public:
    virtual size_t SearchMenu_Size() const = 0;
    virtual void SearchMenu_Search() = 0;
    virtual Entry* SearchMenu_Get(size_t i) = 0;

    bool SearchMenu_NeedsDisplayUpdate() {
	return EntryList_Changed() || Input_Changed() || Visibility_StartOptionChanged();
    }

public:
    virtual void Debug_PrintAllEntries() = 0;
    virtual void Debug_PrintSearchedEntries() = 0;
};
