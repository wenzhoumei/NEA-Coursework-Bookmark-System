#include "file_option_list.hpp"

#include <fstream>
#include <iostream>

bool FileOptionList::LoadOptions() {
    if (!std::filesystem::exists(file_path_)) {
        std::cerr << "Error: File " << file_path_ << " does not exist." << std::endl;
        return false;
    }

    std::ifstream file_stream(file_path_);
    if (!file_stream.is_open()) {
        std::cerr << "Error: Could not open file " << file_path_ << std::endl;
        return false;
    }

    std::string line;

    bool found_invalid = false;
    size_t invalid_count;

    while (std::getline(file_stream, line)) {
        size_t pos = line.find(">");

	std::string name;
	std::string data;

        if (pos == std::string::npos) {
	    while (names_to_data_.contains(name = "INVALID" + std::to_string(invalid_count++)));
	    data = line;
	    found_invalid = true;
        } else {
	    name = line.substr(0, pos);
	    data = line.substr(pos + 1);
	}

	names_to_data_[name] = data;
        option_order_.push_back(name);
    }

    if (found_invalid) {
	WriteOptionsToFile_();
    }

    file_stream.close();
    return true;
}

bool FileOptionList::RemoveOption(const std::string& option_name) {
    if (!OptionList::RemoveOption(option_name)) {
        return false;
    }

    if (!WriteOptionsToFile_()) {
        return false;
    }

    return true;
}

bool FileOptionList::AddOption(const std::string& option_name, const std::string& option_data) {
    if (!OptionList::AddOption(option_name, option_data)) {
        return false;
    }

    if (!WriteOptionsToFile_()) {
        return false;
    }

    return true;
}

bool FileOptionList::InsertOption(size_t pos, const std::string& option_name, const std::string& option_data) {
    if (!OptionList::InsertOption(pos, option_name, option_data)) {
        return false;
    }

    if (!WriteOptionsToFile_()) {
        return false;
    }

    return true;
}

bool FileOptionList::UpdateOption(const std::string& option_name, const std::string& option_data) {
    if (!OptionList::UpdateOption(option_name, option_data)) {
        return false;
    }

    if (!WriteOptionsToFile_()) {
        return false;
    }

    return true;
}

bool FileOptionList::WriteOptionsToFile_() {
    std::ofstream file_stream(file_path_);
    if (!file_stream.is_open()) {
        std::cerr << "Error: Could not open file " << file_path_ << std::endl;
        return false;
    }

    for (const std::string& name : option_order_) {
        file_stream << name << ">" << names_to_data_[name] << std::endl;
    }

    file_stream.close();
    return true;
}
