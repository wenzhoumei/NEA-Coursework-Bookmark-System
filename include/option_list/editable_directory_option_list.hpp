#pragma once

#include "option_list.hpp"
#include "read_directory_option_list.hpp"

#include <filesystem>
#include <string>

class EditableDirectoryOptionList: public ReadDirectoryOptionList {
public:
    using ReadDirectoryOptionList::ReadDirectoryOptionList;

    ModifyStatus Add(const std::wstring& option_string) override;
    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;
    ModifyStatus Remove(size_t pos) override;
    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;

    bool IsBookmarkList() const override { return false; }
    bool Editable() const override { return true; }
};
