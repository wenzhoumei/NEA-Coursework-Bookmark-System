#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "file_option_list.hpp"

class BmkOptionList: public FileOptionList {
public:
    BmkOptionList(std::wstring action, std::wstring location)
	: FileOptionList(action, location)
    {
    }

    bool Load() override;

    ModifyStatus Add(const std::wstring& option_string) override;
    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;
    ModifyStatus Remove(size_t pos) override;
    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;
    ModifyStatus UpdateData(size_t pos, const std::wstring& new_option_string) override;

    bool Contains(const std::wstring& option_string) const override;
    bool Search(const std::wstring& option_string) override;

    std::wstring NameAt(size_t i) const override;
    std::wstring DataAt(size_t i) const override;

    bool IsBookmarkList() override { return true; }
    bool Editable() override { return true; }

protected:
    bool Flush_() override;

    void SplitStringToNameAndData_(const std::wstring& option_string, std::wstring& name, std::wstring& data);
    void RemoveNameInMap_(const std::wstring& name);

    std::unordered_map<std::wstring, std::wstring> Names_To_Data_;
};
