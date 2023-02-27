#pragma once

#include <memory>
#include <string>
#include <vector>

#include "option.hpp"

class MenuData {
    std::unique_ptr<Option> Title;

    std::string MenuDefaultAction = "";

    std::string Input;
    size_t CursorPosition;

    std::vector<std::unique_ptr<Option>> SearchOptions;
    std::vector<int> SearchedOptions;
    size_t SelectedOptionPosition;

    enum { TITLE, OPTION, STATUS_LOG } Selected;

    std::vector<std::string> StatusLog;
};
