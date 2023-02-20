#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <numeric>

#include "entry_list.hpp"

// EASIER IF I IMPLEMENT DICTIONARY string: Entry
// Means I have to process earlier
// Means I have to process or maybe change unprocessed to just splitting by tab
// Called ProcessedLevelOne

// https://stackoverflow.com/questions/1098175/a-stdmap-that-keep-track-of-the-order-of-insertion
// multi index

class VectorEntryList: public EntryList {
protected:
    std::unordered_map<std::wstring, std::unique_ptr<Entry>> entry_dict_;

    std::vector<std::wstring> entries_;
    std::vector<size_t> searched_;

    void SetSearchedToEntries_() {
        searched_.resize(entries_.size());
        std::iota(searched_.begin(), searched_.end(), 0);
    }

    size_t selected_ = 0;
    std::wstring title_;

    bool search_on_ = true;
public:
    VectorEntryList(const std::wstring title): title_(title) { }

    std::wstring GetTitle() override { return title_; }

    int GetSelected() override { return selected_; }

    bool Down() override { 
	if (selected_ >= SearchedSize() - 1) { return false; }

	selected_++;
	return true;
    }

    bool Up() override { 
	if (selected_ <= 0) { return false; }
	
	selected_--;
	return true;
    }

    void ToggleSearch() override { search_on_ = !search_on_; }

    void PrintAllEntries() override {
	for (auto& entry: entries_) {
	    std::wcerr << entry_dict_[entry]->GetString() << std::endl;
	}
    }

    void PrintSearchedEntries() override {
	for (auto& i: searched_) {
	    std::wcerr << entry_dict_[entries_[i]]->GetString() << std::endl;
	}
    }

    void Search(const std::wstring& search) override;

    Entry* AtIndex(int i) override {
	return entry_dict_[entries_[searched_[i]]].get();
    }

    size_t SearchedSize() const override {
	return searched_.size();
    }

    bool RemoveEntry() override {
	entry_dict_.erase(entries_[searched_[selected_]]);
        entries_.erase(entries_.begin() + searched_[selected_]);

	// Update the searched_ vector
	for (auto i = selected_; i < searched_.size(); i++) {
	  searched_[i]--;
	}

	searched_.erase(searched_.begin() + selected_);

	if (selected_ > SearchedSize() - 1) { selected_ = SearchedSize() - 1; }

	return true;
    }

    bool AddEntry(const std::wstring& entry) override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(entry));

	std::wstring entry_name = unprocessed_entry->GetName();
	if (entry_dict_.contains(entry_name)) {
	    std::wcerr << "Error: Entry(" << entry_name << ") already exists" << std::endl;
	    return false;
	}

        entries_.push_back(entry_name);
	entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	searched_.push_back(entries_.size() - 1);

	return true;
    }

    bool InsertEntry(const std::wstring& entry) override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(entry));

	std::wstring entry_name = unprocessed_entry->GetName();

	if (entry_dict_.contains(entry_name)) {
	    std::cerr << "Error: Entry already exists" << std::endl;
	    return false;
	}

	entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	entries_.insert(entries_.begin() + searched_[selected_], entry_name);

	// Update the searched_ vector
	for (auto i = selected_; i < searched_.size(); i++) {
	  searched_[i]++;
	}

	searched_.insert(searched_.begin() + selected_, searched_[selected_] - 1);

	return true;
    }

    bool UpdateEntry(const std::wstring& entry) override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(entry));
	std::wstring entry_name = unprocessed_entry->GetName();

	auto it = std::find(entries_.begin(), entries_.end(), entry_name);
        if (it != entries_.end()) {
	    // Swap selected entry with found entry if entry found
	    std::swap(entries_[searched_[selected_]], *it);
        } else {
	    // Else replace indexed entry
	    entry_dict_.emplace(entry_name, std::move(entry_dict_[entries_[searched_[selected_]]]));
	    entry_dict_.erase(entries_[searched_[selected_]]);
	    entries_[searched_[selected_]] = entry_name;
	}
	return true;
    }
};
