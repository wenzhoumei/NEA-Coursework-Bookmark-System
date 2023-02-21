#pragma once

#include "vector_entry_list.hpp"
#include <filesystem>
#include <fstream>

class FileVectorEntryList: public VectorEntryList {
protected:
    std::filesystem::path file_path_;

    bool WriteToFile_() {
        std::wofstream file(file_path_);

        if (!file) {
            std::cerr << "Error: Unable to open file for writing: " << file_path_ << std::endl;
	    // TODO Handle if there is an error
	    return false;
        }

        for (const std::wstring& entry : entries_) {
            file << entry_dict_[entry]->GetString() << std::endl;
        }

	file.close();
	return true;
    }

public:
    FileVectorEntryList(std::wstring title, std::filesystem::path file_path)
	: VectorEntryList(title), file_path_(file_path)
    {
    }

    bool Initialize() override {
	std::wifstream file(file_path_);

	if (!file) {
	    // File doesn't exist, so create it
	    std::ofstream new_file(file_path_);
	    if (!new_file) {
		std::cerr << "Error: Unable to create file: " << file_path_ << std::endl;
		return false;
	    }
	} else {
	    // File exists, so read entries from it
	    while (std::getline(file, input_text_)) {
		VectorEntryList::AddEntry();
	    }

	    input_text_ = L"";
	}

	SetSearchedToEntries_();
	return EntryList::Initialize();
    }

    bool UpdateMenu() override {
	return WriteToFile_();
    }
};
