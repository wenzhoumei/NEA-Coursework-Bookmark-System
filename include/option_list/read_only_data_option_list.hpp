#pragma once
#include "data_option_list.hpp"

/**
 * @brief Read only version of data option list
 */
class ReadOnlyDataOptionList: public DataOptionList {
public:
    using DataOptionList::DataOptionList;

    bool Editable() const override { return false; }
};
