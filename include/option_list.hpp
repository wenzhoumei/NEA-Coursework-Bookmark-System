#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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

    virtual bool Contains(const std::string& option_string) {
	auto it = std::find(options_.begin(), options_.end(), option_string);
	if (it != options_.end()) {
	    return true;
	} else {
	    return false;
	}
    }

    bool Search(const std::string& input_text);

    const std::vector<int>& GetSearched() { return searched_; }

    std::string At(size_t i) {
	return options_[searched_[i]];
    }

    void Print() {
	std::cout << "-------------" << std::endl;
	for (const auto& option: options_) {
	    std::cout << option << std::endl;
	}
    }

    const bool IsBookmarkList = false;
protected:
    std::vector<std::string> options_;
    std::vector<int> searched_;
};
