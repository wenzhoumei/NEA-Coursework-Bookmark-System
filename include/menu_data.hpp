#pragma once

#include <memory>
#include <string>
#include <vector>

#include "option_list.hpp"

class MenuData {
public:
    MenuData(std::wstring title, std::wstring action, std::unique_ptr<OptionList> option_list)
	: Title(title), Action(action), Option_List(std::move(option_list))
    {
    }

    std::wstring Title;
    std::wstring Action;

    std::wstring Input = L"";
    size_t Cursor_Position = 0;

    enum Mode { SEARCH, INSERT, EDIT, DATA } Mode = SEARCH;

    std::unique_ptr<OptionList> Option_List;

    size_t SelectedOptionPosition = 0;
    enum { OPTION, STATUS_LOG } Selected;

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
};
