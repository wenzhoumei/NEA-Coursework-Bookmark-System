#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include "file_retriever.hpp"
#include "option_list.hpp"

class DataOptionList: public OptionList {
protected:
    std::filesystem::path File_Path_;
    std::unique_ptr<Retriever> Option_Retriever = nullptr;


public:
    bool Flush() {
        std::wofstream file(File_Path_);

        if (!file) {
            std::cerr << "Error: Unable to open file for writing: " << File_Path_ << std::endl;
            return false;
        }

        for (const std::wstring& entry : options_) {
            file << entry << std::endl;
        }

        return true;
    }

    bool SuccessfullyLoaded = false;
    void Load(std::filesystem::path file_path) override {
	File_Path_ = file_path;
	Option_Retriever = std::make_unique<FileRetriever>(FileRetriever(file_path));

	if (!Option_Retriever->Load()) {
	    SuccessfullyLoaded = false;
	    return;
	}

	options_ = Option_Retriever->GetData();

	SuccessfullyLoaded = true;
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

    const bool HasData = false;
    const bool Editable = true;
};
