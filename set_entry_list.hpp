#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <numeric>

#include "entry_list.hpp"

class SetEntryList: public EntryList {
protected:
    void SetCurPos_(size_t new_cur_pos) {
	Previous.CursorPosition = cur_x_;
	cur_x_ = new_cur_pos;
    }

    void SetSelPos_(size_t new_sel_pos) {
	Previous.SelectedPosition = selected_;
	selected_ = new_sel_pos;
    }

    std::wstring GetSearchedEntryNameAtIndex_(size_t i) {
	std::wstring key = *std::next(searched_.begin(), i);
	return key;
    }

public:
    SetEntryList(const std::wstring title): title_(title) { }

    std::wstring GetTitle() override { return title_; }
    std::wstring GetInputText() override { return input_text_; }

    void SetInputText(const std::wstring& input_text) override { 
	input_text_ = input_text;
	
	SetCurPos_(input_text.size());
	NeedsUpdate.Input = true;

	Search();
    }

    void AddCharToInputText(const wchar_t &input_char) override {
	if (!iswprint(input_char)) return;

	input_text_.insert(cur_x_, 1, input_char);
	NeedsUpdate.Input = true;

	SetCurPos_(cur_x_ + 1);

	Search();
    }

    void RemoveCharFromInputText() override {
	if (input_text_ == L"") return;

	NeedsUpdate.Input = true;

	Left();
	input_text_.pop_back();

	Search();
    }

    size_t GetSelectedIndex() override { return selected_; }
    size_t GetCursorPosition() override { return cur_x_; }

    void Down() override { 
	if (selected_ >= SearchedSize() - 1) { return; }

	SetSelPos_(selected_ + 1);
    }

    void Up() override { 
	if (selected_ <= 0) { return; }
	
	SetSelPos_(selected_ - 1);
    }

    void Left() override { 
	if (cur_x_ <= 0) { return; }

	SetCurPos_(cur_x_ - 1);
    }

    void Right() override { 
	if (cur_x_ >= input_text_.size()) { return; }
	
	SetCurPos_(cur_x_ + 1);
    }

    void ToggleSearch() override { search_on_ = !search_on_; }

    void PrintAllEntries() override {
	for (const auto& [key, val]: entry_dict_) {
	    std::wcerr << val->GetString() << std::endl;
	}
    }

    void PrintSearchedEntries() override {
	for (auto& i: searched_) {
	    std::wcerr << entry_dict_[i]->GetString() << std::endl;
	}
    }

    void Search() override;

    Entry* GetSearchedEntryAtIndex(size_t i) override {
	std::wstring key = *std::next(searched_.begin(), i);
	return entry_dict_[key].get();
    }

    Entry* GetSelectedEntry() override {
	return GetSearchedEntryAtIndex(selected_);
    }

    size_t SearchedSize() const override {
	return searched_.size();
    }

    void RemoveEntry() override {
	if (SearchedSize() == 0) { return; }
	    
	std::wstring remove_name = GetSearchedEntryNameAtIndex_(selected_);
	entry_dict_.erase(remove_name);
        searched_.erase(std::find(searched_.begin(), searched_.end(), remove_name));

	if (selected_ > SearchedSize() - 1) { selected_ = SearchedSize() - 1; }

	NeedsUpdate.Menu = true;
    }

    void AddEntry() override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(input_text_));

	std::wstring name_to_add = unprocessed_entry->GetName();
	if (entry_dict_.contains(name_to_add)) {
	    std::wcerr << "Error: Entry(" << name_to_add << ") already exists" << std::endl;
	    return;
	}

	entry_dict_.emplace(name_to_add, std::move(unprocessed_entry));
	Search();

	NeedsUpdate.Menu = true;
    }

    void InsertEntry() override {
	AddEntry();
    }

    void UpdateEntry() override {
	if (SearchedSize() == 0) { return; }

	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(input_text_));
	std::wstring entry_name = unprocessed_entry->GetName();

	std::wstring name_to_update = GetSearchedEntryNameAtIndex_(selected_);
        if (!entry_dict_.contains(name_to_update)) {
	    // TODO deal with this
	    return;
        } else {
	    entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	    
	    entry_dict_.erase(name_to_update);
	}

	Search();

	NeedsUpdate.Menu = true;
    }

protected:
    std::unordered_map<std::wstring, std::unique_ptr<Entry>> entry_dict_;

    std::vector<std::wstring> searched_;

    void SetSearchedToEntries_() {
	searched_.clear();

	for (const auto& [key, val] : entry_dict_) {
	    searched_.push_back(key);
	}
    }

    size_t selected_ = 0;
    std::wstring title_;
    std::wstring input_text_;

    size_t cur_x_ = 0;

    bool search_on_ = true;
};
