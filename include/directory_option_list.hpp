#pragma once

#include "option_list.hpp"

#include <filesystem>
#include <string>

class DirectoryOptionList: OptionList {
public:
    DirectoryOptionList(std::filesystem::path dir_path) {}
    bool Add(const std::string& option_string) override;
    bool Remove(const std::string& option_string) override;
    bool Update(const std::string& option_string, const std::string& new_option_string) override;
    bool Insert(size_t pos, const std::string& option_string) override;

    const bool IsBookmark = false;
protected:
    std::filesystem::path dir_path;
};
