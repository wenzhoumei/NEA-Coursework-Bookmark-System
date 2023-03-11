#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "data_option_list.hpp"

class ReadOnlyDataOptionList: public DataOptionList {
public:
    using DataOptionList::DataOptionList;

    bool Editable() override { return true; }
};
