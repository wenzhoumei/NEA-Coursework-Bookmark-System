#include "directory_option_list.hpp"

#include <iostream>
#include <filesystem>

bool DirectoryOptionList::LoadOptions() {
    // Clear any existing options
    names_to_data_.clear();
    option_order_.clear();

    // Load options from the directory
    std::filesystem::path directory(directory_path_);
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        return false;
    }

    for (const auto& file : std::filesystem::directory_iterator(directory)) {
        if (file.is_regular_file()) {
            // Add the file as an option
            std::string file_name = file.path().filename().string();
            AddOption(file_name, "");
        }
    }

    return true;
}

bool DirectoryOptionList::RemoveOption(const std::string& option_name) {
    if (!ContainsOption(option_name)) {
        return false;
    }

    // Delete the file associated with the option
    std::string file_path = directory_path_ + "/" + option_name;
    std::filesystem::remove(file_path);

    // Remove the option from the mapping and order vector
    names_to_data_.erase(option_name);
    option_order_.erase(std::remove(option_order_.begin(), option_order_.end(), option_name), option_order_.end());

    return true;
}

bool DirectoryOptionList::AddOption(const std::string& option_name, const std::string& option_data) {
    if (ContainsOption(option_name)) {
        return false;
    }

    // Create an empty file with the option name
    std::string file_path = directory_path_ + "/" + option_name;
    std::ofstream file(file_path);
    if (!file) {
        return false;
    }

    // Add the option to the mapping and order vector
    names_to_data_[option_name] = option_data;
    option_order_.push_back(option_name);

    return true;
}

bool DirectoryOptionList::InsertOption(size_t pos, const std::string& option_name, const std::string& option_data) {
    if (ContainsOption(option_name)) {
        return false;
    }

    // Insert the option name into the order vector
    if (pos > option_order_.size()) {
        pos = option_order_.size();
    }
    option_order_.insert(option_order_.begin() + pos, option_name);

    // Create an empty file with the option name
    std::string file_path = directory_path_ + "/" + option_name;
    std::ofstream file(file_path);
    if (!file) {
        return false;
    }

    // Add the option to the mapping
    names_to_data_[option_name] = option_data;

    return true;
}

bool DirectoryOptionList::UpdateOption(const std::string& option_name, const std::string& option_data) {
    if (!ContainsOption(option_name)) {
        return AddOption(option_name, option_data);
    }

    // Update the option data in the mapping
    names_to_data_[option_name] = option_data;

    // Swap the option's position with the next option
    size_t index = std::find(option_order_.begin(), option_order_.end(), option_name) - option_order_.begin();
    if (index != option_order_.size() - 1) {
        std::swap(option_order_[index], option_order_[index + 1]);
    }

    return true;
}

std::vector<std::string> DirectoryOptionList::SubList(size_t start
