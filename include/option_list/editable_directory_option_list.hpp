#pragma once

#include "option_list.hpp"
#include "read_directory_option_list.hpp"

#include <filesystem>
#include <string>

/**
 * @brief Version of ReadDirectoryOptionList that is editable
 */
class EditableDirectoryOptionList: public ReadDirectoryOptionList {
public:
    using ReadDirectoryOptionList::ReadDirectoryOptionList;

    /**
     * @brief Creates file with that name
     */
    ModifyStatus Add(const std::wstring& option_string) override;

    /**
     * @brief Creates file with that name
     */
    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;

    /**
     * @brief Removes file with name at pos
     */
    ModifyStatus Remove(size_t pos) override;

    /**
     * @brief Updates filename with same logic as in OptionList
     */
    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;

    bool IsBookmarkList() const override { return false; }
    bool Editable() const override { return true; }
};
