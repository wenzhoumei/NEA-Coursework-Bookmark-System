#pragma once

#include <memory>
#include <string>
#include <vector>

#include "option_list/option_list.hpp"

class MenuData {
public:
    MenuData(OptionList* option_list)
	: Option_List(std::move(option_list))
    {
    }

    std::wstring Title;

    std::wstring Input = L"";
    size_t Cursor_Position = 0;

    enum Editing { NAME, DATA } Editing = NAME;
    enum Mode { SEARCH, INSERT, EDIT } Mode = SEARCH;

    std::unique_ptr<OptionList> Option_List;

    size_t SelectedOptionPosition = 0;

    std::unique_ptr<OptionList> Status_Log;

    struct {
	bool Title = true;
	bool Input = true;
	bool Option_List = true;
	bool Selected = true;
	bool Status_Log = true;

	void Reset() {
	    Title = Input = Option_List = Selected = Status_Log = false;
	}
    } Changed;

    std::wstring SelectedName() const {
	return Option_List->NameAt(SelectedOptionPosition);
    }

    std::wstring SelectedData() const {
	return Option_List->DataAt(SelectedOptionPosition);
    }

    bool IsSearchListEmpty() const {
	return Option_List->SearchedSize() == 0;
    }
};
