#include "option_list/read_directory_option_list.hpp"

bool ReadDirectoryOptionList::Load() {
    Directory_Retriever_ = std::make_unique<DirectoryRetriever>(DirectoryRetriever(Location_));

    if (!Directory_Retriever_->Load()) {
	return false;
    }

    Location_ = std::filesystem::absolute(Location_).wstring();
    std::filesystem::current_path(Location_); //setting path

    Options_All_ = Directory_Retriever_->GetData();

    Search(L"");
    return true;
}
