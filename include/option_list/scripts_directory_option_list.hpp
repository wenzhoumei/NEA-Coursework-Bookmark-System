#pragma once

#include "option_list.hpp"
#include "editable_directory_option_list.hpp"

#include <filesystem>
#include <string>

/**
 * @brief Connects to option list to executable directory
 */
class ScriptsDirectoryOptionList: public EditableDirectoryOptionList {
public:
    using EditableDirectoryOptionList::EditableDirectoryOptionList;

    /**
     * @brief Makes file added executable and checks if this is the scripts directory, if so updates it
     */
    ModifyStatus Add(const std::wstring& option_string) override;

    /**
     * @brief Makes file inserted executable and checks if this is the scripts directory, if so updates it
     */
    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;

    /**
     * @brief Checks if this is the scripts directory, if so updates it
     */
    ModifyStatus Remove(size_t pos) override;

    /**
     * @brief Checks if this is the scripts directory, if so updates it
     */
    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;

    bool IsBookmarkList() const override { return false; }
    bool Editable() const override { return true; }
};
