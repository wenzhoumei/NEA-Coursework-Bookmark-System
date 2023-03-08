#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include "file_option_list.hpp"
#include "log.hpp"

class DataOptionList: public FileOptionList {
protected:
    std::filesystem::path File_Path_;

public:
    bool Flush() override {
        std::wofstream file(File_Path_);

        if (!file) {
	    Log::Instance().Error(1) << "Unable to open file for writing: " << File_Path_;
            return false;
        }

        for (const std::wstring& option_name : options_) {
            file << option_name << std::endl;
        }

        return true;
    }

    void Load(const std::filesystem::path& file_path) override {
	File_Path_ = file_path;
	File_Retriever_ = std::make_unique<FileRetriever>(FileRetriever(file_path));

	if (!File_Retriever_->Load()) {
	    SuccessfullyLoaded_ = false;
	    return;
	}

	options_ = File_Retriever_->GetData();

	SuccessfullyLoaded_ = true;
    }

    bool Add(const std::wstring& option_string) override {
	return OptionList::Add(option_string) && Flush();
    }

    bool Remove(size_t pos) override {
	return OptionList::Remove(pos) && Flush();
    }

    bool Update(size_t pos, const std::wstring& new_option_string) override {
	return OptionList::Update(pos, new_option_string) && Flush();
    }

    bool Insert(size_t pos, const std::wstring& option_string) override {
	return OptionList::Insert(pos, option_string) && Flush();
    }

    const bool IsBookmarkList = false;
    const bool Editable = true;
};
