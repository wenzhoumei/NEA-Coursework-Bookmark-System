#pragma once

#include <vector>
#include <string>

class Retriever {
protected:
    std::vector<std::wstring> data_;

public:
    virtual bool Load() = 0;
    const std::vector<std::wstring>& GetData() const { return data_; };
};
