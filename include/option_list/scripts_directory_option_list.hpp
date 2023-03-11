#pragma once

#include "option_list.hpp"
#include "editable_directory_option_list.hpp"

#include <filesystem>
#include <string>

class ScriptsDirectoryOptionList: public EditableDirectoryOptionList {
public:
    using EditableDirectoryOptionList::EditableDirectoryOptionList;

    ModifyStatus Add(const std::wstring& option_string) override;

    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;

    ModifyStatus Remove(size_t pos) override;

    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;

    bool IsBookmarkList() override { return false; }
    bool Editable() override { return true; }
};
