#pragma once

#include <memory>
#include <string>
#include "entry.hpp"

class EntryList {
private:
    std::string SplitByLast_(std::string& str, char del);
    std::string SplitByFirst_(std::string& str, char del);
    std::string SplitByIndex_(std::string& str, size_t del_pos);

protected:
    /*
    std::unique_ptr<Entry> ProcessEntry(std::unique_ptr<Entry> entry);
    */

public:
    // Return false if any of these fail else return true
    virtual bool RemoveEntry(const int& selected) = 0;
    virtual bool AddEntry(const std::string& entry) = 0;
    virtual bool InsertEntry(const std::string& entry, const int& selected) = 0;
    virtual bool UpdateEntry(const std::string& entry, const int& selected) = 0;

    virtual Entry* AtIndex(int i) = 0;

    virtual size_t SearchedSize() const = 0;

    virtual void Search(const std::string& search) = 0;

    virtual void PrintAllEntries() = 0;
    virtual void PrintSearchedEntries() = 0;
};
