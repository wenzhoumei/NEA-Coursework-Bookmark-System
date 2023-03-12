#pragma once

#include <vector>
#include <string>
#include <filesystem>

class Retriever {
protected:
    std::vector<std::wstring> data_;

public:
    virtual bool Load() = 0;
    virtual ~Retriever() {};
    const std::vector<std::wstring>& GetData() const { return data_; };
};
