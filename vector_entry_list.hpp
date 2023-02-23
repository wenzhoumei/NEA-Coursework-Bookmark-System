#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <numeric>

#include "entry_list.hpp"

class VectorEntryList: public EntryList {

public:
    VectorEntryList(const std::wstring title) {
	Title_text_ = title;
    }


    void Debug_PrintAllEntries() override {
	for (auto& entry: entries_) {
	    std::wcerr << entry_dict_[entry]->GetString() << std::endl;
	}
    }

    void Debug_PrintSearchedEntries() override {
	for (auto& i: searched_) {
	    std::wcerr << entry_dict_[entries_[i]]->GetString() << std::endl;
	}
    }

    void SearchMenu_Search() override;

    Entry* SearchMenu_Get(size_t i) override {
	return entry_dict_[entries_[searched_[i]]].get();
    }

    size_t SearchMenu_Size() const override {
	return searched_.size();
    }

    void EntryList_RemoveEntry() override {
	if (SearchMenu_Size() == 0) { return; }

	entry_dict_.erase(entries_[searched_[Selected_selected_]]);
        entries_.erase(entries_.begin() + searched_[Selected_selected_]);

	// Update the searched_ vector
	for (auto i = Selected_selected_; i < searched_.size(); i++) {
	  searched_[i]--;
	}

	searched_.erase(searched_.begin() + Selected_selected_);

	Selected_ReduceIfAboveMaximum_();

	Mode_Set(SEARCH);
	EntryList_changed_ = true;
    }

    void EntryList_AddEntry() override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(Input_text_));

	std::wstring entry_name = unprocessed_entry->GetName();
	if (entry_dict_.contains(entry_name)) {
	    std::wcerr << "Error: Entry(" << entry_name << ") already exists" << std::endl;
	    return;
	}

        entries_.push_back(entry_name);
	entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	searched_.push_back(entries_.size() - 1);

	Mode_Set(SEARCH);
	EntryList_changed_ = true;
    }

    void EntryList_InsertEntry() override {
	if (SearchMenu_Size() == 0 || Selected_selected_ == SearchMenu_Size()) {
	    EntryList_AddEntry();
	    return;
	}

	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(Input_text_));

	std::wstring entry_name = unprocessed_entry->GetName();

	if (entry_dict_.contains(entry_name)) {
	    std::cerr << "Error: Entry already exists" << std::endl;
	    return;
	}

	entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	entries_.insert(entries_.begin() + searched_[Selected_selected_], entry_name);

	// Update the searched_ vector
	for (auto i = Selected_selected_; i < searched_.size(); i++) {
	  searched_[i]++;
	}

	searched_.insert(searched_.begin() + Selected_selected_, searched_[Selected_selected_] - 1);

	Mode_Set(SEARCH);
	EntryList_changed_ = true;
    }

    void EntryList_UpdateEntry() override {
	if (SearchMenu_Size() == 0) { 
	    EntryList_AddEntry();
	}

	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(Input_text_));
	std::wstring entry_name = unprocessed_entry->GetName();

	auto it = std::find(entries_.begin(), entries_.end(), entry_name);
        if (it != entries_.end()) {
	    // Swap selected entry with found entry if entry found
	    std::swap(entries_[searched_[Selected_selected_]], *it);
        } else {
	    auto it2 = entry_dict_.find(entries_[searched_[Selected_selected_]]);
	    entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	    entry_dict_.erase(it2);
	    entries_[searched_[Selected_selected_]] = entry_name;
	}

	Mode_Set(SEARCH);
	EntryList_changed_ = true;
    }

protected:
    std::unordered_map<std::wstring, std::unique_ptr<Entry>> entry_dict_;

    std::vector<std::wstring> entries_;
    std::vector<size_t> searched_;

    void SetSearchedToEntries_() {
        searched_.resize(entries_.size());
        std::iota(searched_.begin(), searched_.end(), 0);
    }
};
