#pragma once

#include <memory>
#include <string>
#include "entry.hpp"

class EntryList {
private:
    std::wstring SplitByLast_(std::wstring& str, wchar_t del);
    std::wstring SplitByFirst_(std::wstring& str, wchar_t del);
    std::wstring SplitByIndex_(std::wstring& str, size_t del_pos);

protected:
    /*
    std::unique_ptr<Entry> ProcessEntry(std::unique_ptr<Entry> entry);
    */

public:
    virtual std::wstring GetTitle() = 0;

    virtual size_t GetSelectedIndex() = 0;
    virtual bool Up() = 0;
    virtual bool Down() = 0;

    virtual std::wstring GetInputText() = 0;
    virtual void SetInputText(const std::wstring& input_str) = 0;

    // Return false if any of these fail else return true
    virtual bool RemoveEntry() = 0;
    virtual bool AddEntry() = 0;
    virtual bool InsertEntry() = 0;
    virtual bool UpdateEntry() = 0;

    virtual Entry* GetSelectedEntry() = 0;
    virtual Entry* GetEntryAtIndex(size_t i) = 0;

    virtual size_t SearchedSize() const = 0;

    virtual void ToggleSearch() = 0;

    virtual void Search() = 0;

    virtual void PrintAllEntries() = 0;
    virtual void PrintSearchedEntries() = 0;
};
