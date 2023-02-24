#pragma once

#include "set_entry_list.hpp"
#include <filesystem>

class DirectorySetEntryList: public SetEntryList {
protected:
    std::filesystem::path directory_path_;
public:
    DirectorySetEntryList(std::wstring title, std::filesystem::path directory_path)
	: SetEntryList(title), directory_path_(directory_path)
    {
    }

    bool Update_Initialize() override {
	if (!std::filesystem::is_directory(directory_path_)) { return false; }

	// File exists, so read entries from it
	// Load all file names in the specified directory
	for (const auto& entry : std::filesystem::directory_iterator(directory_path_)) {
	    if (entry.is_regular_file()) {
		std::wstring file_name = entry.path().filename().wstring();
		entry_dict_.emplace(file_name, std::make_unique<UnprocessedEntry>(UnprocessedEntry(file_name)));
	    }
	}

	Input_text_ = L"";

	SetSearchedToEntries_();
	return EntryList::Update_Initialize();
    }
};
