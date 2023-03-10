#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "data_option_list.hpp"

class ReadOnlyDataOptionList: public DataOptionList {
public:
    ReadOnlyDataOptionList(std::wstring action, std::wstring location)
	: DataOptionList(action, location) {}

    bool Editable() override { return true; }
};
