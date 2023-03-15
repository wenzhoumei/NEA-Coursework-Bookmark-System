#pragma once

#include "option_list/option_list.hpp"

/**
 * @brief Menu only for input
 */
class InputOnlyOptionList: public OptionList {
public:
    using OptionList::OptionList;

    bool Load() override { return true; };
};
