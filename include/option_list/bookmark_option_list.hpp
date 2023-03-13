#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "option_list/file_option_list.hpp"

/**
 * @brief Makes sure each name is unique and hides data
 */
class BmkOptionList: public FileOptionList {
public:
    using FileOptionList::FileOptionList;

    /**
     * @brief Loads each line, splits first data delimiter to get name and data and adds
     */
    bool Load() override;

    /**
     * @brief Splits line into data and name then adds name to Options_All_ and maps data to name
     */
    ModifyStatus Add(const std::wstring& option_string) override;

    /**
     * @brief Splits line into data and name then adds name to Options_All_ and maps data to name
     */
    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;

    /**
     * @brief Removes name at pos from both Options_All_ and Names_To_Data_
     */
    ModifyStatus Remove(size_t pos) override;

    /**
     * @brief When name is changed, data remains the same, also doesn't do anything if sequence contains a >
     */
    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;

    /**
     * @brief Updates data at pos to new_data in both Options_All_ and Names_To_Data_

     */
    ModifyStatus UpdateData(size_t pos, const std::wstring& new_data) override;

    /**
     * @brief Faster contains, can just check std::map
     */
    bool Contains(const std::wstring& option_string) const override;

    /**
     * @brief OptionList::Search but not searching anything after first >
     */
    bool Search(const std::wstring& option_string) override;

    /**
     * @brief Name at pos i
     */
    std::wstring NameAt(size_t i) const override;

    /**
     * @brief Data at pos i
     */
    std::wstring DataAt(size_t i) const override;

    bool IsBookmarkList() const override { return true; }
    bool Editable() const override { return true; }

protected:
    /**
     * @brief Different flush as must reconstruct option string
     */
    bool Flush_() override;

    /**
     * @brief Finds first > to in option string splitting into name and data respectively, if no > found, data is ""
     */
    void SplitStringToNameAndData_(const std::wstring& option_string, std::wstring& name, std::wstring& data);

    /**
     * @brief Used to find name and remove it from map
     */
    void RemoveNameInMap_(const std::wstring& name);

    std::unordered_map<std::wstring, std::wstring> Names_To_Data_;
};
