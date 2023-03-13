#pragma once

#include "file_option_list.hpp"

class DataOptionList: public FileOptionList {
public:
    using FileOptionList::FileOptionList;

    bool Flush_() override;
    bool Load() override;

    bool Editable() const override { return true; }
};
