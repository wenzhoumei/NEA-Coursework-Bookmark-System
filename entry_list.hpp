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
    struct {
	bool Title;
	bool Input;
	bool Menu;

	void Reset() {
	    Title = Input = Menu = false;
	};
    } NeedsUpdate;

    struct {
	size_t CursorPosition;
	size_t SelectedPosition;
    } Previous;

    virtual bool Initialize() {
	NeedsUpdate.Title = true;
	NeedsUpdate.Input = true;
	NeedsUpdate.Menu = true;

	Previous.CursorPosition = -1;
	Previous.SelectedPosition = -1;
	return true;
    }

    virtual bool UpdateTitle() { return true; }
    virtual bool UpdateMenu() { return true; }

    virtual std::wstring GetTitle() = 0;

    virtual size_t GetSelectedIndex() = 0;
    virtual size_t GetCursorPosition() = 0;

    virtual void Up() = 0;
    virtual void Down() = 0;

    virtual void Left() = 0;
    virtual void Right() = 0;

    virtual std::wstring GetInputText() = 0;

    virtual void SetInputText(const std::wstring& input_char) = 0;
    virtual void AddCharToInputText(const wchar_t& input_char) = 0;
    virtual void RemoveCharFromInputText() = 0;

    // Return false if any of these fail else return true
    virtual void RemoveEntry() = 0;
    virtual void AddEntry() = 0;
    virtual void InsertEntry() = 0;
    virtual void UpdateEntry() = 0;

    virtual Entry* GetSelectedEntry() = 0;
    virtual Entry* GetEntryAtIndex(size_t i) = 0;

    virtual size_t SearchedSize() const = 0;

    virtual void ToggleSearch() = 0;

    virtual void Search() = 0;

    virtual void PrintAllEntries() = 0;
    virtual void PrintSearchedEntries() = 0;
};
