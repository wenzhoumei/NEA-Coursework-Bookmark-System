#pragma once

#include "option_list.hpp"
#include "directory_retriever.hpp"

#include <filesystem>
#include <string>

class ReadDirectoryOptionList: public OptionList {
    std::unique_ptr<DirectoryRetriever> Directory_Retriever_ = nullptr;

public:
    using OptionList::OptionList;

    bool Load() override {
	Directory_Retriever_ = std::make_unique<DirectoryRetriever>(DirectoryRetriever(Location_));

	if (!Directory_Retriever_->Load()) {
	    return false;
	}

	Options_All_ = Directory_Retriever_->GetData();

	Search(L"");
	return true;
    }

    const bool Editable = false;
    const bool HasData = false;
};
