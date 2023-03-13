#pragma once

#include "option_list.hpp"
#include "retriever/directory_retriever.hpp"

#include <filesystem>
#include <string>

class ReadDirectoryOptionList: public OptionList {
    std::unique_ptr<DirectoryRetriever> Directory_Retriever_ = nullptr;

public:
    using OptionList::OptionList;

    bool Load() override;

    bool IsBookmarkList() const override { return false; }
    bool Editable() const override { return false; }

    std::wstring DataAt(size_t i) const override {
	std::filesystem::current_path(Location_); //setting path
	return std::filesystem::absolute(Options_All_[Options_Indexes_Searched[i]]).wstring();
    }
};
