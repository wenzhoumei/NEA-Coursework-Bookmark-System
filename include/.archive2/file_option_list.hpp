#pragma once

#include "option_list.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>

class FileOptionList: public OptionList {
public:
    FileOptionList(std::filesystem::path file_path): file_path_(file_path) {};

    bool LoadOptions() override;

    bool RemoveOption(const std::string& option_name) override;
    bool AddOption(const std::string& option_name, const std::string& option_data) override;

    bool InsertOption(size_t pos, const std::string& option_name, const std::string& option_data) override;
    bool UpdateOption(const std::string& option_name, const std::string& option_data) override;

protected:
    std::filesystem::path file_path_;

    bool WriteOptionsToFile_();
};
