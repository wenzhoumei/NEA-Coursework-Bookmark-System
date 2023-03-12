#pragma once

#include <memory>
#include <string>
#include <vector>

#include "option_list/option_list.hpp"

class MenuData {
public:
    static std::stack<std::pair<std::wstring, std::wstring>> History;

    MenuData(OptionList* option_list);

    std::wstring Title;

    std::wstring Input = L"";
    size_t Cursor_Position = 0;

    enum Editing { NAME, DATA } Editing = NAME;
    enum Mode { SEARCH, INSERT, EDIT } Mode = SEARCH;

    std::unique_ptr<OptionList> Option_List;

    size_t SelectedOptionPosition = 0;

    struct Changed {
	bool Title = true;
	bool Input = true;
	bool Option_List = true;
	bool Selected = true;
	bool Status_Log = true;

	void Reset();
    } Changed;

    std::wstring SelectedName() const;
    std::wstring SelectedData() const;

    bool IsSearchListEmpty() const;
};
