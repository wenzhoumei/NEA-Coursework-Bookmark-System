#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "bookmark_option_list.hpp"
#include "parser.hpp"

void BmkOptionList::Load(const std::filesystem::path& file_path) {
    File_Path_ = file_path;
    File_Retriever_ = std::make_unique<FileRetriever>(FileRetriever(file_path));

    if (!File_Retriever_->Load()) {
	SuccessfullyLoaded_ = false;
	return;
    }

    for (auto option_string: File_Retriever_->GetData()) {
	std::wstring name;
	std::wstring data;
	SplitStringToNameAndData_(option_string, name, data);
	options_.push_back(name);
	names_to_data_[name] = data;
    }

    SuccessfullyLoaded_ = true;
}

bool BmkOptionList::Flush() {
    std::wofstream file(File_Path_);

    if (!file) {
	Log::Instance().Error(1) << "Unable to open file for writing: " << File_Path_;
	return false;
    }

    for (const std::wstring& option_name : options_) {
	file << option_name << Parser::Instance().Data.Delimiter << names_to_data_[option_name] << std::endl;

	//Log::Instance().Debug() << "name:" << option_name;
	//Log::Instance().Debug() << "data:" << names_to_data_[option_name];
    }

    return true;
}

bool BmkOptionList::Add(const std::wstring& option_string) {
    std::wstring name;
    std::wstring data;
    SplitStringToNameAndData_(option_string, name, data);

    Log::Instance().Debug() << "adding name:" << name;
    Log::Instance().Debug() << "data:" << data;

    bool successfully_added = OptionList::Add(name);

    if (!successfully_added) {
	Log::Instance().Debug() << "Not successfully_added";
	return false;
    } else {
	names_to_data_[name] = data;
	return Flush();
    }
}

bool BmkOptionList::Insert(size_t pos, const std::wstring& option_string) {
    std::wstring name;
    std::wstring data;
    SplitStringToNameAndData_(option_string, name, data);

    bool successfully_inserted = OptionList::Insert(pos, name);

    if (!successfully_inserted) {
	Log::Instance().Debug() << "Not successfully_added";
	return false;
    } else {
	names_to_data_[name] = data;
	return Flush();
    }
}

bool BmkOptionList::Remove(size_t pos) {
    std::wstring name = NameAt(pos);
    bool successfully_removed = FileOptionList::Remove(pos);

    if (successfully_removed) {
	RemoveName_(name);
	return false;
    }

    return successfully_removed;
}

bool BmkOptionList::Update(size_t pos, const std::wstring& new_option_string) {
    //TODO wrong should be new name
    std::wstring name;
    std::wstring data;
    SplitStringToNameAndData_(new_option_string, name, data);

    bool successfully_updated = OptionList::Update(pos, name); // Won't do anything it failed

    if (successfully_updated) {
	names_to_data_[name] = data;
	return Flush();
    } else {
	return false;
    }
}

bool BmkOptionList::UpdateData(size_t pos, const std::wstring& new_data) {
    names_to_data_[NameAt(pos)] = new_data;
    return Flush();
}

bool BmkOptionList::Contains(const std::wstring& option_string) const {
    return names_to_data_.contains(option_string);
}

void BmkOptionList::Search(const std::wstring& option_string) {
    std::wstring name;
    std::wstring data;
    SplitStringToNameAndData_(option_string, name, data);

    OptionList::Search(name);
}

std::wstring BmkOptionList::NameAt(size_t i) {
    return options_[searched_[i]];
}


std::wstring BmkOptionList::OptionStringAt(size_t i) {
    return NameAt(i) + Parser::Instance().Data.Delimiter + names_to_data_[NameAt(i)];
}

std::wstring BmkOptionList::DataAt(size_t i) {
    return names_to_data_[NameAt(i)];
}

void BmkOptionList::SplitStringToNameAndData_(const std::wstring& option_string, std::wstring& name, std::wstring& data) {
    size_t data_del_pos = option_string.find_first_of(Parser::Instance().Data.Delimiter);
    if (data_del_pos == std::wstring::npos) {
	name = option_string;
	data = L"";
    } else {
	name = option_string.substr(0, data_del_pos);
	data = option_string.substr(data_del_pos + 1);
    }
}

void BmkOptionList::RemoveName_(const std::wstring& name) {
    auto it = names_to_data_.find(name);

    names_to_data_.erase(it);
    options_.erase(std::remove(options_.begin(), options_.end(), name), options_.end());
}
