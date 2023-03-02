#include "option_list.hpp"
#include <algorithm>

bool OptionList::ContainsOption(const std::string& option_name) const {
    return names_to_data_.contains(option_name);
}

bool OptionList::RemoveOption(const std::string& option_name) {
    auto it = names_to_data_.find(option_name);
    if (it == names_to_data_.end()) {
        return false;
    }
    names_to_data_.erase(it);
    option_order_.erase(std::remove(option_order_.begin(), option_order_.end(), option_name), option_order_.end());
    return true;
}

bool OptionList::AddOption(const std::string& option_name, const std::string& option_data) {
    if (ContainsOption(option_name)) {
        return false;
    }
    names_to_data_[option_name] = option_data;
    option_order_.push_back(option_name);
    return true;
}

bool OptionList::InsertOption(size_t pos, const std::string& option_name, const std::string& option_data) {
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

bool OptionList::UpdateOption(const std::string& option_name, const std::string& option_data) {
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

std::vector<std::string> OptionList::SubList(size_t start, size_t end) const {
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

std::string OptionList::DataAt(std::string option_name) const {
    return names_to_data_.at(option_name);
}

size_t OptionList::Size() const {
    return option_order_.size();
}
