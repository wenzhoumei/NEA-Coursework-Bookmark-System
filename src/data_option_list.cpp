#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include "option_list/data_option_list.hpp"
#include "config_directory.hpp"

bool DataOptionList::Flush_() {
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

bool DataOptionList::Load()  {
    SetLocationPath();
    File_Retriever_ = std::make_unique<FileRetriever>(FileRetriever(Location_));

    if (!std::filesystem::is_regular_file(Location_)) {
	std::wstring test_default_location = (ConfigDirectory::Instance().GetOptionListsDirectoryPath()/std::filesystem::path(Location_)).wstring();
	if (!File_Retriever_->Load()) { return false; }
	else { Location_ = test_default_location; }
    } else if (!File_Retriever_->Load()) {
	File_Retriever_ = std::make_unique<FileRetriever>(FileRetriever(Location_));
	if (!File_Retriever_->Load()) { return false; }
    }

    Options_All_ = File_Retriever_->GetData();

    Search(L"");
    return true;
}
