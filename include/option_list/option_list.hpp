#pragma once

#include <vector>
#include <string>

/**
 * @brief Abstract class that represents an list of options, this is used to load a menu in the MenuTUI class
 *
 * Contains default virtual functions that deals with storage in program, inherited classes add connection to backend(files/directories) and add can change operation of certain functions
 */
class OptionList {
public:
    OptionList(std::wstring action_out_of_here_, std::wstring action_to_here_, std::wstring location)
	: Action_Out_Of_Here_(action_out_of_here_), Action_To_Here_(action_to_here_), Location_(location)
    {
    }

    virtual ~OptionList() {};
    virtual bool Load() = 0;

    struct ModifyStatus {
	bool Modified; ///> Has Options_All_ been changed
	bool BackendError; ///> Has error occurred in backend, i.e. writing to file/directory
    };

    /**
     * @brief Adds string to end of menu
     *
     * @param pos Position inside Options_Indexes_Searched_ to add it at
     */
    virtual ModifyStatus Add(const std::wstring& option_string);

    /**
     * @brief Function that will be binded to Enter in Insert mode, inserts current input text at pos, shifting all later entries down
     *
     * @param pos Position inside Options_Indexes_Searched_ to insert it at
     * @param option_string Option string to insert
     */
    virtual ModifyStatus Insert(size_t pos, const std::wstring& option_string);

    /**
     * @brief Removes option at if it exists
     *
     * @param pos Position inside Options_Indexes_Searched_ to remove it at
     */
    virtual ModifyStatus Remove(size_t pos);

    /**
     * @brief Changes option at position pos to new_option_string, if new_option_string exists, swaps these two options
     *
     * @param pos Position inside Options_Indexes_Searched_ to update it at
     * @param new_option_string Option string to update to
     */
    virtual ModifyStatus Update(size_t pos, const std::wstring& new_option_string);

    /**
     * @brief Changes data at position pos to new_data
     *
     * @param pos Position inside Options_Indexes_Searched_ to update it at
     * @param option_string Data to update to
     */
    virtual ModifyStatus UpdateData(size_t pos, const std::wstring& new_data);

    /**
     * @brief Checks if the option list contains the option_string
     *
     * @param option_string Option string to check
     * @return Does it contain option string
     */
    virtual bool Contains(const std::wstring& option_string) const;

    /**
     * @brief Updates Options_Indexes_Searched_ with matching exact, prefix and substring matches in that priority
     *
     * @param option_string Option string to check
     * @return Does it contain option string
     */
    virtual bool Search(const std::wstring& input_text);

    /**
     * @brief Gets vector containing indexes of all found strings from Search
     */
    const std::vector<int>& GetSearched() { return Options_Indexes_Searched_; }

    /**
     * @brief Gets size of vector containing indexes of all found strings from Search
     */
    size_t SearchedSize() { return Options_Indexes_Searched_.size(); }

    /**
     * @brief Gets option string pointed to by Options_Indexes_Searched_
     */
    virtual std::wstring OptionStringAt(size_t i) const;

    /**
     * @brief Gets name pointed to by Options_Indexes_Searched_
     */
    virtual std::wstring NameAt(size_t i) const {
	return Options_All_[Options_Indexes_Searched_[i]];
    }

    /**
     * @brief Gets data pointed to by Options_Indexes_Searched_
     */
    virtual std::wstring DataAt(size_t i) const {
	return Options_All_[Options_Indexes_Searched_[i]];
    }

    /**
     * @brief Gets action executed to get here, used for rendering title
     */
    std::wstring GetActionToHere() const { return Action_To_Here_; };

    /**
     * @brief Gets action for execution of option string
     */
    std::wstring GetActionOutOfHere() const { return Action_Out_Of_Here_; };

    /**
     * @brief Gets location of fully defined file or directory or other that corresponds with this option list to save the data
     */
    std::wstring GetLocation() const { return Location_; };

    /**
     * @brief Should the menu hide everything including and after >
     */
    virtual bool IsBookmarkList() const { return false; }

    /**
     * @brief Is this type of option list editable
     */
    virtual bool Editable() const { return false; }

protected:
    std::vector<std::wstring> Options_All_; ///< A list of all the options
    std::vector<int> Options_Indexes_Searched_; ///< A list of all options after a Search
    std::wstring Action_Out_Of_Here_; ///< Action that executes on selected/inputted option
    std::wstring Action_To_Here_; ///< Destination action that got here
    std::wstring Location_; ///< File/directory/other that the list of values was found
};
