#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include "file_option_list.hpp"

class DataOptionList: public FileOptionList {
public:
    DataOptionList(std::wstring action, std::wstring location)
	: FileOptionList(action, location)
    {
    }

    bool Flush_() override {
	std::filesystem::path path(Location_);
        std::wofstream file(path);

        if (!file) {
            return false;
        }

        for (const std::wstring& option_name : Options_All_) {
            file << option_name << std::endl;
        }

        return true;
    }

    bool Load() override {
	File_Retriever_ = std::make_unique<FileRetriever>(FileRetriever(Location_));

	if (!File_Retriever_->Load()) {
	    return false;
	}

	Options_All_ = File_Retriever_->GetData();

	Search(L"");
	return true;
    }

    bool Editable() override { return true; }
};
