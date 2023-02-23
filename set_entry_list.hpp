#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <numeric>

#include "entry_list.hpp"

class SetEntryList: public EntryList {
protected:
    std::wstring GetSearchedEntryNameAtIndex_(size_t i) {
	std::wstring key = *std::next(searched_.begin(), i);
	return key;
    }

public:
    void Debug_PrintAllEntries() override {
	for (const auto& [key, val]: entry_dict_) {
	    std::wcerr << val->GetString() << std::endl;
	}
    }

    void Debug_PrintSearchedEntries() override {
	for (auto& i: searched_) {
	    std::wcerr << entry_dict_[i]->GetString() << std::endl;
	}
    }

    void SearchMenu_Update() override;

    size_t SearchMenu_Size() const override {
	return searched_.size();
    }

    void EntryList_RemoveEntry() override {
	if (SearchMenu_Size() == 0) { return; }
	    
	std::wstring remove_name = GetSearchedEntryNameAtIndex_(Selected_selected_);
	entry_dict_.erase(remove_name);
        searched_.erase(std::find(searched_.begin(), searched_.end(), remove_name));

	if (Selected_selected_ > SearchMenu_Size() - 1) { Selected_selected_ = SearchMenu_Size() - 1; }

	SearchMenu_needs_update_ = true;
    }

    void EntryList_AddEntry() override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(Input_text_));

	std::wstring name_to_add = unprocessed_entry->GetName();
	if (entry_dict_.contains(name_to_add)) {
	    std::wcerr << "Error: Entry(" << name_to_add << ") already exists" << std::endl;
	    return;
	}

	entry_dict_.emplace(name_to_add, std::move(unprocessed_entry));
	SearchMenu_Update();

	SearchMenu_needs_update_ = true;
    }

    void EntryList_InsertEntry() override {
	EntryList_AddEntry();
    }

    void EntryList_UpdateEntry() override {
	if (SearchMenu_Size() == 0) { return; }

	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(Input_text_));
	std::wstring entry_name = unprocessed_entry->GetName();

	std::wstring name_to_update = GetSearchedEntryNameAtIndex_(Selected_selected_);
        if (!entry_dict_.contains(name_to_update)) {
	    // TODO deal with this
	    return;
        } else {
	    entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	    
	    entry_dict_.erase(name_to_update);
	}

	SearchMenu_Update();

	SearchMenu_needs_update_ = true;
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
};
