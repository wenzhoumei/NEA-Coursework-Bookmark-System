#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include "option_list.hpp"

class BmkOptionList: public FileOptionList {
public:
    BmkOptionList(std::filesystem::path file_path)
	: FileOptionList(file_path)
    {
    }

    bool UpdateData(size_t pos, std::string new_data) {
    }

    const bool IsBookmark = true;
};


class OptionList {
public:
    virtual bool Load() = 0;

    virtual bool Add(const std::string& option_string) {
	if (Contains(option_string)) { return false; }

	options_.push_back(option_string);
	return true;
    };

    virtual bool Insert(size_t pos, const std::string& option_string) {
	if (pos > options_.size()) {
	    // Position out of range
	    return false;
	}

	if (Contains(option_string)) { return false; }

	// Insert at the specified position
	options_.insert(options_.begin() + pos, option_string);
	return true;
    }

    virtual bool Remove(size_t pos) {
	options_.erase(options_.begin() + pos);
	return true;
    };

    virtual bool Update(size_t pos, const std::string& new_option_string) {
	if (pos >= options_.size()) {
	    // Position out of range
	    return false;
	}

	// Check if the new option string already exists
	auto it = std::find(options_.begin(), options_.end(), new_option_string);
	if (it != options_.end()) {
	    // New option string already exists, swap with the old one
	    std::swap(options_[pos], *it);
	    return true;
	}

	// Replace the option string at the specified position with the new one
	options_[pos] = new_option_string;
	return true;
    }

    virtual bool Contains(const std::string& option_string) = 0;

    bool Search();
    const std::vector<std::string>& GetSearched();

    std::string operator[](size_t i) {
	return options_[i];
    }

    const bool IsBookmark = false;
protected:
    std::vector<std::string> options_;
    std::vector<int> searched_;
};
bool BmkOptionList::ContainsOption(const std::string& option_name) const {
    return names_to_data_.contains(option_name);
}

bool BmkOptionList::RemoveOption(const std::string& option_name) {
    auto it = names_to_data_.find(option_name);
    if (it == names_to_data_.end()) {
        return false;
    }
    names_to_data_.erase(it);
    option_order_.erase(std::remove(option_order_.begin(), option_order_.end(), option_name), option_order_.end());
    return true;
}

bool BmkOptionList::AddOption(const std::string& option_name, const std::string& option_data) {
    if (ContainsOption(option_name)) {
        return false;
    }
    names_to_data_[option_name] = option_data;
    option_order_.push_back(option_name);
    return true;
}

bool BmkOptionList::InsertOption(size_t pos, const std::string& option_name, const std::string& option_data) {
    if (ContainsOption(option_name)) {
        return false;
    }
    if (pos > option_order_.size()) {
        pos = option_order_.size();
    }
    names_to_data_[option_name] = option_data;
    option_order_.insert(option_order_.begin() + pos, option_name);
    return true;
}

bool BmkOptionList::UpdateOption(const std::string& option_name, const std::string& option_data) {
    if (!ContainsOption(option_name)) {
        AddOption(option_name, option_data);
        return true;
    }
    std::string& data = names_to_data_[option_name];
    data = option_data;
    auto it = std::find(option_order_.begin(), option_order_.end(), option_name);
    if (it != option_order_.end() - 1) {
        std::swap(*it, *(it + 1));
    }
    return true;
}

std::vector<std::string> BmkOptionList::SubList(size_t start, size_t end) const {
    if (start > option_order_.size()) {
        start = option_order_.size();
    }
    if (end > option_order_.size()) {
        end = option_order_.size();
    }
    if (start > end) {
        return {};
    }
    return std::vector<std::string>(option_order_.begin() + start, option_order_.begin() + end);
}

std::string BmkOptionList::DataAt(std::string option_name) const {
    return names_to_data_.at(option_name);
}

size_t BmkOptionList::Size() const {
    return option_order_.size();
}
