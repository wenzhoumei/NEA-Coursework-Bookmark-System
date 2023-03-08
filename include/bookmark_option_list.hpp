#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "file_option_list.hpp"

class BmkOptionList: public FileOptionList {
public:
    void Load(const std::filesystem::path& file_path) override;

    bool Flush() override;
    bool Add(const std::wstring& option_string) override;
    bool Insert(size_t pos, const std::wstring& option_string) override;
    bool Remove(size_t pos) override;
    bool Update(size_t pos, const std::wstring& new_option_string) override;
    bool Contains(const std::wstring& option_string) const override;

    bool UpdateData(size_t pos, const std::wstring& new_option_string) override;
    void Search(const std::wstring& option_string) override;

    std::wstring NameAt(size_t i) override;
    std::wstring OptionStringAt(size_t i) override;
    std::wstring DataAt(size_t i) override;

    const bool IsBookmarkList = true;
    const bool Editable = true;

protected:
    std::unordered_map<std::wstring, std::wstring> names_to_data_;
    void SplitStringToNameAndData_(const std::wstring& option_string, std::wstring& name, std::wstring& data);
    void RemoveName_(const std::wstring& name);
};
