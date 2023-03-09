#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "data_option_list.hpp"

class ReadOnlyDataOptionList: public DataOptionList {
public:
    ReadOnlyDataOptionList(std::wstring action, std::wstring location)
	: DataOptionList(action, location) {}

    const bool HasData = false;
    const bool Editable = false;
};
