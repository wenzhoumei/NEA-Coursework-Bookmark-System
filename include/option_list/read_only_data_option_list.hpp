#pragma once
#include "data_option_list.hpp"

class ReadOnlyDataOptionList: public DataOptionList {
public:
    using DataOptionList::DataOptionList;

    bool Editable() const override { return false; }
};
