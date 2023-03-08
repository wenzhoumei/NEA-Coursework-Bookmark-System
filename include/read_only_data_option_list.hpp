#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "data_option_list.hpp"

class ReadOnlyDataOptionList: public DataOptionList {
public:
    const bool HasData = false;
    const bool Editable = false;
};
