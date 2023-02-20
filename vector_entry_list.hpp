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
    std::unordered_map<std::string, std::unique_ptr<Entry>> entry_dict_;

    std::vector<std::string> entries_;
    std::vector<int> searched_;

    void SetSearchedToEntries_() {
        searched_.resize(entries_.size());
        std::iota(searched_.begin(), searched_.end(), 0);
    }

public:
    VectorEntryList() {
	SetSearchedToEntries_();
    }

    void PrintAllEntries() override {
	for (auto& entry: entries_) {
	    std::cerr << entry_dict_[entry]->GetString() << std::endl;
	}
    }

    void PrintSearchedEntries() override {
	for (auto& i: searched_) {
	    std::cerr << entry_dict_[entries_[i]]->GetString() << std::endl;
	}
    }

    void Search(const std::string& search) override;

    Entry* AtIndex(int i) override {
	return entry_dict_[entries_[searched_[i]]].get();
    }

    size_t SearchedSize() const override {
	return searched_.size();
    }

    bool RemoveEntry(const int& selected) override {
        entries_.erase(entries_.begin() + searched_[selected]);
	entry_dict_.erase(entries_[searched_[selected]]);

	return true;
    }

    bool AddEntry(const std::string& entry) override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(entry));

	std::string entry_name = unprocessed_entry->GetName();
	if (entry_dict_.contains(entry_name)) {
	    std::cerr << "Error: Entry(" << entry_name << ") already exists" << std::endl;
	    return false;
	}

        entries_.push_back(entry_name);
	entry_dict_.emplace(entry_name, std::move(unprocessed_entry));

	return true;
    }

    bool InsertEntry(const std::string& entry, const int& selected) override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(entry));

	std::string entry_name = unprocessed_entry->GetName();

	if (entry_dict_.contains(entry_name)) {
	    std::cerr << "Error: Entry already exists" << std::endl;
	    return false;
	}

	entry_dict_.emplace(entry_name, std::move(unprocessed_entry));
	entries_.insert(entries_.begin() + searched_[selected], entry_name);

	return true;
    }

    bool UpdateEntry(const std::string& entry, const int& selected) override {
	std::unique_ptr<Entry> unprocessed_entry = std::make_unique<UnprocessedEntry>(UnprocessedEntry(entry));
	std::string entry_name = unprocessed_entry->GetName();

	auto it = std::find(entries_.begin(), entries_.end(), entry_name);
        if (it != entries_.end()) {
	    // Swap it with entries[i] if entry found
	    std::swap(entries_[searched_[selected]], *it);
        } else {
	    // Else replace indexed entry
	    entry_dict_.emplace(entry_name, std::move(entry_dict_[entries_[searched_[selected]]]));
	    entry_dict_.erase(entries_[searched_[selected]]);
	    entries_[searched_[selected]] = entry_name;
	}
	return true;
    }
};
