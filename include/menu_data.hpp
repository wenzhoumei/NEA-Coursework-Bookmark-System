#pragma once

#include <memory>
#include <string>
#include <vector>
#include <stack>

#include "option_list/option_list.hpp"

/**
 * @brief Representation of menu, contains all information about current state of menu
 *
 * MenuView uses this to render menu and MenuController changes this and uses this in its logic
 */
class MenuData {
public:
    static std::stack<std::pair<std::wstring, std::wstring>> History;

    MenuData(OptionList* option_list);

    std::wstring Title; ///> String in title bar

    std::wstring Input = L""; ///> String in input box
    size_t Cursor_Position = 0;

    enum Editing { NAME, DATA } Editing = NAME;
    enum Mode { SEARCH, INSERT, EDIT } Mode = SEARCH;

    std::unique_ptr<OptionList> Option_List; ///> Initializer of menu data, defines functionality of menu

    size_t Selected_Option_Position = 0;

    /**
     * Updated by MenuController and resetted by MenuView
     * Should be only instance a class other than MenuController directly changes MenuData class
     */
    struct Changed {
	bool Title = true;
	bool Input = true;
	bool Option_List = true;
	bool Selected = true;
	bool Status_Log = true;

	/**
	 * @brief Set all to true
	 */
	void SetAll();

	/**
	 * @brief Set all to false
	 */
	void SetNone();
    } Changed;

    /**
     * @return Name which is displayed part of selected string
     */
    std::wstring SelectedName() const;

    /**
     * @return Returns option string to be executed
     */
    std::wstring SelectedOptionString() const;

    /**
     * @return Selected data
     */
    std::wstring SelectedData() const;

    /**
     * @return If search list is empty
     */
    bool IsSearchListEmpty() const;
};
