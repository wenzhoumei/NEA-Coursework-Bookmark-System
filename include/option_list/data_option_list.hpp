#pragma once

#include "file_option_list.hpp"

/**
 * @brief Each line in file option list represents data of option string
 */
class DataOptionList: public FileOptionList {
public:
    using FileOptionList::FileOptionList;

    /**
     * @brief Flushes each string in OptionList::Options_All_ vector into file
     */
    bool Flush_() override;

    /**
     * @brief Loads each line in OptionList::Options_All_ into file
     */
    bool Load() override;

    bool Editable() const override { return true; }
};
