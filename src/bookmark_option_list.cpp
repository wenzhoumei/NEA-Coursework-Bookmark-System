#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "option_list/bookmark_option_list.hpp"
#include "parser.hpp"
#include "log.hpp"

bool BmkOptionList::Load() {
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

    for (auto option_string: File_Retriever_->GetData()) {
	std::wstring name;
	std::wstring data;
	SplitStringToNameAndData_(option_string, name, data);
	Options_All_.push_back(name);
	Names_To_Data_[name] = data;
    }

    Search(L"");
    return true;
}

bool BmkOptionList::Flush_() {
    // Constructor of wofstream object doesn't take wstring as argument
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrow_location = converter.to_bytes(Location_);
    std::wofstream file(narrow_location);

    if (!file) {
	my::log.Error(ExitCode::WriteError) << "Unable to open file for writing: " << Location_;
	return false;
    }

    for (const std::wstring& option_name : Options_All_) {
	file << option_name << Parser::Instance().Data.Delimiter << Names_To_Data_[option_name] << std::endl;
    }

    return true;
}

OptionList::ModifyStatus BmkOptionList::Add(const std::wstring& option_string) {
    std::wstring name;
    std::wstring data;
    SplitStringToNameAndData_(option_string, name, data);

    OptionList::ModifyStatus m_s = OptionList::Add(name);

    if (m_s.BackendError) {
	return { m_s.Modified, true };
    } else {
	if (m_s.Modified) {
	    Names_To_Data_[name] = data;
	    m_s.BackendError = !Flush_();
	}

	return m_s;
    }
}

OptionList::ModifyStatus BmkOptionList::Insert(size_t pos, const std::wstring& option_string) {
    std::wstring name;
    std::wstring data;
    SplitStringToNameAndData_(option_string, name, data);

    OptionList::ModifyStatus m_s = OptionList::Insert(pos, name);

    if (m_s.BackendError) {
	return { m_s.Modified, true };
    } else {
	if (m_s.Modified) {
	    Names_To_Data_[name] = data;
	    m_s.BackendError = !Flush_();
	}

	return m_s;
    }
}

OptionList::ModifyStatus BmkOptionList::Remove(size_t pos) {
    RemoveNameInMap_(NameAt(pos));
    OptionList::ModifyStatus m_s = OptionList::Remove(pos);

    if (m_s.BackendError) {
	return { true, true };
    } else {
	if (m_s.Modified) {
	    m_s.BackendError = !Flush_();
	}

	return m_s;
    }
}

OptionList::ModifyStatus BmkOptionList::Update(size_t pos, const std::wstring& new_option_string) {
    if (pos >= Options_All_.size()) { my::log.Error(ExitCode::LogicError) << "Can't remove, out of range"; }

    if (std::find(new_option_string.begin(), new_option_string.end(), (Parser::Instance().Data.Delimiter)) != new_option_string.end()) {
	my::log.Warning() << "Invalid sequence, contains >, nothing done" << std::endl;
	return { false, false };
    }

    // Check if the new option string already exists
    auto it = std::find(Options_All_.begin(), Options_All_.end(), new_option_string);
    if (it != Options_All_.end()) {
	// New name already exists, swap with the old one
	std::swap(Options_All_[pos], *it);
    } else {
	std::wstring data = DataAt(pos);
	// Replace the option string at the specified position with the new one
	Options_All_[pos] = new_option_string;
	Names_To_Data_[new_option_string] = data;
    }

    return { true, !Flush_() };
}

OptionList::ModifyStatus BmkOptionList::UpdateData(size_t pos, const std::wstring& new_data) {
    Names_To_Data_[NameAt(pos)] = new_data;

    return { true, !Flush_() };
}

bool BmkOptionList::Contains(const std::wstring& option_string) const {
    return Names_To_Data_.contains(option_string);
}

bool BmkOptionList::Search(const std::wstring& option_string) {
    std::wstring name;
    std::wstring data;
    SplitStringToNameAndData_(option_string, name, data);

    return OptionList::Search(name);
}

std::wstring BmkOptionList::NameAt(size_t i) const {
    return Options_All_.at(Options_Indexes_Searched[i]);
}

std::wstring BmkOptionList::DataAt(size_t i) const {
    return Names_To_Data_.at(NameAt(i));
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

void BmkOptionList::RemoveNameInMap_(const std::wstring& name) {
    auto it = Names_To_Data_.find(name);

    Names_To_Data_.erase(it);
}
