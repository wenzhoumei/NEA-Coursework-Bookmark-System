#pragma once

#include <memory>
#include <string>
#include <vector>

#include "option_list.hpp"

class MenuData {
public:
    MenuData(const std::string title, const std::string& action, std::unique_ptr<OptionList> option_list)
	: Title(title), Action(action), Option_List(std::move(option_list))
    {
    }

    std::string Title;
    std::string Action;

    std::string Input = "";
    size_t Cursor_Position = 0;

    enum Mode { SEARCH, INSERT, EDIT, DATA } Mode = SEARCH;

    std::unique_ptr<OptionList> Option_List;

    size_t SelectedOptionPosition = 0;
    enum { OPTION, STATUS_LOG } Selected;

    std::unique_ptr<OptionList> Status_Log;

    struct {
	bool Title = false;
	bool Input = false;
	bool Option_List = false;
	bool Selected = false;
	bool Status_Log = false;

	void Reset() {
	    Title = Input = Option_List = Selected = Status_Log = false;
	}
    } Changed;
};
