#pragma once

#include <string>
#include "entry_list.hpp"

class Menu {
public:
    std::wstring title;
    std::wstring input;
    EntryList entries;
};
