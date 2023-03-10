#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <assert.h>
#include "log.hpp"

class OptionList {
public:
    OptionList(std::wstring action, std::wstring location)
	: Action_(action), Location_(location)
    {
    }

    virtual bool Load() = 0;

    struct ModifyStatus {
	bool Modified; // Has Options_All_ been changed
	bool BackendError; // Has error occurred in backend, i.e. writing to file/directory
    };

    virtual ModifyStatus Add(const std::wstring& option_string) {
	if (Contains(option_string)) { return { false, false }; }

	Options_All_.push_back(option_string);
	return { true, false };
    };

    virtual ModifyStatus Insert(size_t pos, const std::wstring& option_string) {
	if (pos > Options_All_.size()) { Log::Instance().Error(9) << "Can't insert, out of range"; }

	if (Contains(option_string)) { return { false, false }; }

	// Insert at the specified position
	Options_All_.insert(Options_All_.begin() + pos, option_string);
	return { true, false };
    }

    virtual ModifyStatus Remove(size_t pos) {
	if (pos >= Options_All_.size()) { Log::Instance().Error(9) << "Can't remove, out of range"; }
	if (Options_All_.size() == 0) { return { false, false}; } // Can't remove if empty

	Options_All_.erase(Options_All_.begin() + pos);
	return { true, false };
    };

    virtual ModifyStatus Update(size_t pos, const std::wstring& new_option_string) {
	if (pos >= Options_All_.size()) { Log::Instance().Error(9) << "Can't update, out of range"; }

	// Check if the new option string already exists
	auto it = std::find(Options_All_.begin(), Options_All_.end(), new_option_string);
	if (it != Options_All_.end()) {
	    // New option string already exists, swap with the old one
	    std::swap(Options_All_[pos], *it);
	} else {
	    // Replace the option string at the specified position with the new one
	    Options_All_[pos] = new_option_string;
	}

	return { true, false };
    }

    virtual ModifyStatus UpdateData(size_t pos, const std::wstring& new_data) { 
	Log::Instance().Error(9) << "This should not be called";
	return { false, false }; // Doesn't matter
    };

    virtual bool Contains(const std::wstring& option_string) const {
	auto it = std::find(Options_All_.begin(), Options_All_.end(), option_string);
	if (it != Options_All_.end()) {
	    return true;
	} else {
	    return false;
	}
    }

    virtual bool Search(const std::wstring& input_text);

    const std::vector<int>& GetSearched() { return Options_Indexes_Searched; }

    size_t SearchedSize() { return Options_Indexes_Searched.size(); }

    virtual std::wstring NameAt(size_t i) const {
	return Options_All_[Options_Indexes_Searched[i]];
    }

    virtual std::wstring DataAt(size_t i) const {
	return Options_All_[Options_Indexes_Searched[i]];
    }

    void Print() {
	std::cout << "-------------" << std::endl;
	for (const auto& option: Options_All_) {
	    std::wcout << option << std::endl;
	}
    }

    std::wstring GetAction() const { return Action_; };
    std::wstring GetLocation() const { return Location_; };

    virtual bool IsBookmarkList() { return false; }
    virtual bool Editable() { return false; }
protected:
    std::vector<std::wstring> Options_All_;
    std::vector<int> Options_Indexes_Searched;
    std::wstring Action_;
    std::wstring Location_;
};
