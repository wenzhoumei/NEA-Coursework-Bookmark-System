#pragma once

#include <vector>
#include <string>

class OptionList {
public:
    OptionList(std::wstring action_out_of_here_, std::wstring action_to_here_, std::wstring location)
	: Action_Out_Of_Here_(action_out_of_here_), Action_To_Here_(action_to_here_), Location_(location)
    {
    }

    virtual ~OptionList() {};
    virtual bool Load() = 0;

    struct ModifyStatus {
	bool Modified; // Has Options_All_ been changed
	bool BackendError; // Has error occurred in backend, i.e. writing to file/directory
    };

    virtual ModifyStatus Add(const std::wstring& option_string);
    virtual ModifyStatus Insert(size_t pos, const std::wstring& option_string);
    virtual ModifyStatus Remove(size_t pos);
    virtual ModifyStatus Update(size_t pos, const std::wstring& new_option_string);

    virtual ModifyStatus UpdateData(size_t pos, const std::wstring& new_data);

    virtual bool Contains(const std::wstring& option_string) const;

    virtual bool Search(const std::wstring& input_text);

    const std::vector<int>& GetSearched() { return Options_Indexes_Searched; }

    size_t SearchedSize() { return Options_Indexes_Searched.size(); }

    virtual std::wstring OptionStringAt(size_t i) const;

    virtual std::wstring NameAt(size_t i) const {
	return Options_All_[Options_Indexes_Searched[i]];
    }

    virtual std::wstring DataAt(size_t i) const {
	return Options_All_[Options_Indexes_Searched[i]];
    }

    std::wstring GetActionToHere() const { return Action_To_Here_; };
    std::wstring GetActionOutOfHere() const { return Action_Out_Of_Here_; };

    std::wstring GetLocation() const { return Location_; };

    virtual bool IsBookmarkList() const { return false; }
    virtual bool Editable() const { return false; }

protected:
    std::vector<std::wstring> Options_All_; ///< A list of all the options
    std::vector<int> Options_Indexes_Searched; ///< A list of all options after a Search
    std::wstring Action_Out_Of_Here_; ///< Action that executes on selected/inputted option
    std::wstring Action_To_Here_; ///< Destination action that got here
    std::wstring Location_; ///< File/directory/other that the list of values was found
};
