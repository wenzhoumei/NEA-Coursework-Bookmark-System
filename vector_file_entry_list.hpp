#pragma once

#include "vector_entry_list.hpp"
#include <filesystem>
#include <fstream>

class VectorFileEntryList: public VectorEntryList {
protected:
    std::filesystem::path file_path_;

    bool WriteToFile_() {
        std::ofstream file(file_path_);

        if (!file) {
            std::cerr << "Error: Unable to open file for writing: " << file_path_ << std::endl;
            return false;
        }

        for (const std::string& entry : entries_) {
            file << entry_dict_[entry]->GetString() << std::endl;
        }

	file << std::flush;
	file.close();

        return true;
    }

public:
    VectorFileEntryList(std::string title, std::filesystem::path file_path)
	: VectorEntryList(title), file_path_(file_path)
    {
	std::ifstream file(file_path_);

	if (!file) {
	    // File doesn't exist, so create it
	    std::ofstream new_file(file_path_);
	    if (!new_file) {
		std::cerr << "Error: Unable to create file: " << file_path_ << std::endl;
		return;
	    }
	} else {
	    // File exists, so read entries from it
	    std::string line;
	    while (std::getline(file, line)) {
		AddEntry(line);
	    }
	}

	SetSearchedToEntries_();
    }

    bool RemoveEntry() override {
	return VectorEntryList::RemoveEntry() && WriteToFile_();
    }

    bool AddEntry(const std::string& entry) override {
        return VectorEntryList::AddEntry(entry) && WriteToFile_();
    }

    bool UpdateEntry(const std::string& entry) override {
        return VectorEntryList::UpdateEntry(entry) && WriteToFile_();
    }

    bool InsertEntry(const std::string& entry) override {
        return VectorEntryList::InsertEntry(entry) && WriteToFile_();
    }
};
