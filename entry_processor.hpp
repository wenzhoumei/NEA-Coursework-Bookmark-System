#pragma once

#include <string>
#include <memory>
#include "entry.hpp"

class EntryProcessor {
private:
    static std::string SplitByLast_(std::string& str, char del) {
	size_t last_del_pos = str.find_last_of(del);
	if (last_del_pos == std::string::npos) {
	    // Delimiter not found, return empty string
	    return "";
	} else {
	    // Split string into two parts
	    std::string first_part = str.substr(0, last_del_pos);
	    std::string end_part = str.substr(last_del_pos + 1);
	    // Update input string to be the first part
	    str = first_part;
	    // Return the end part
	    return end_part;
	}
    }

    static std::string SplitByFirst_(std::string& str, char del) {
	size_t first_del_pos = str.find(del);
	if (first_del_pos == std::string::npos) {
	    // Delimiter not found, return empty string
	    return "";
	} else {
	    // Split string into two parts
	    std::string first_part = str.substr(0, first_del_pos);
	    std::string end_part = str.substr(first_del_pos + 1);
	    // Update input string to be the end part
	    str = end_part;
	    // Return the first part
	    return first_part;
	}
    }

public:
    static std::unique_ptr<Entry> CreateEntry(std::string str) {
	std::string str_copy = std::string(str);

	std::string data = SplitByFirst_(str_copy, '\t');
	std::string script = SplitByLast_(str_copy, '|');

	return std::make_unique<DefaultEntry>(str, script, data);
    }

};
