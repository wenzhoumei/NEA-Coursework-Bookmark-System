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
    virtual std::string GetTitle() = 0;

    virtual int GetSelected() = 0;
    virtual bool Up() = 0;
    virtual bool Down() = 0;

    // Return false if any of these fail else return true
    virtual bool RemoveEntry() = 0;
    virtual bool AddEntry(const std::string& entry) = 0;
    virtual bool InsertEntry(const std::string& entry) = 0;
    virtual bool UpdateEntry(const std::string& entry) = 0;

    virtual Entry* AtIndex(int i) = 0;

    virtual size_t SearchedSize() const = 0;

    virtual void ToggleSearch() = 0;

    virtual void Search(const std::string& search) = 0;

    virtual void PrintAllEntries() = 0;
    virtual void PrintSearchedEntries() = 0;
};
