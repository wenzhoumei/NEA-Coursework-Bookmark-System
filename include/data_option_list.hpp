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
    DataOptionList(std::wstring action, std::wstring location)
	: FileOptionList(action, location)
    {
    }

    bool Flush_() override {
        std::wofstream file(File_Path_);

        if (!file) {
	    Log::Instance().Error(1) << "Unable to open file for writing: " << File_Path_;
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

    const bool IsBookmarkList = false;
    const bool Editable = true;
};
