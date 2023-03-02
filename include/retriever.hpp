#pragma once

#include <vector>
#include <string>

class Retriever {
protected:
    std::vector<std::string> data_;

public:
    virtual bool Load() = 0;
    const std::vector<std::string>& GetData() const { return data_; };
};
