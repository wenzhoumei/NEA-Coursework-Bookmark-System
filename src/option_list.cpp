#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <numeric>

#include "log.hpp"
#include "parser.hpp"

#include "option_list/option_list.hpp"

OptionList::ModifyStatus OptionList::Add(const std::wstring& option_string) {
    if (Contains(option_string)) { return { false, false }; }

    Options_All_.push_back(option_string);
    return { true, false };
};

OptionList::ModifyStatus OptionList::Insert(size_t pos, const std::wstring& option_string) {
    if (pos > Options_All_.size()) {
	Log::Instance().Info() << "Can't insert, out of range";
	return { false, false };
    }

    if (Contains(option_string)) { return { false, false }; }

    // Insert at the specified position
    Options_All_.insert(Options_All_.begin() + pos, option_string);
    return { true, false };
}

OptionList::ModifyStatus OptionList::Remove(size_t pos) {
    if (pos >= Options_All_.size()) {
	Log::Instance().Info() << "Can't remove, out of range";
	return { false, false };
    }

    if (Options_All_.size() == 0) {
	my::log.Info() << "Can't remove, out of range";
	return { false, false };
    }

    Options_All_.erase(Options_All_.begin() + pos);
    return { true, false };
};

OptionList::ModifyStatus OptionList::Update(size_t pos, const std::wstring& new_option_string) {
    if (pos >= Options_All_.size()) { my::log.Error(ExitCode::LogicError) << "Can't update, out of range"; }

    // Check if the new option string already exists
    auto it = std::find(Options_All_.begin(), Options_All_.end(), new_option_string);
    if (it != Options_All_.end()) {
	// New option string already exists, swap with the old one
	std::swap(Options_All_[pos], *it);
    } else {
	// Replace the option string at the specified position with the new one
	Options_All_[pos] = new_option_string;
    }

    return { true, false };
}

std::wstring OptionList::OptionStringAt(size_t i) const {
    if (IsBookmarkList()) {
	if (DataAt(i) == L"") { return NameAt(i); }
	else { return NameAt(i) + Parser::Instance().Data.Delimiter + DataAt(i); }
    } else {
	return NameAt(i);
    }
}

OptionList::ModifyStatus OptionList::UpdateData(size_t pos, const std::wstring& new_data) { 
    Log::Instance().Error(9) << "This should not be called";
    return { false, false }; // Doesn't matter
};

bool OptionList::Contains(const std::wstring& option_string) const {
    auto it = std::find(Options_All_.begin(), Options_All_.end(), option_string);
    if (it != Options_All_.end()) {
	return true;
    } else {
	return false;
    }
}

bool OptionList::Search(const std::wstring& input_text)
{
    Options_Indexes_Searched.clear();

    std::vector<std::wstring> input_tokens;
    const std::wstring SPLIT_DELIMITERS = L" \t.-~|,@>";
    boost::split(input_tokens, input_text, boost::is_any_of(SPLIT_DELIMITERS), boost::token_compress_on);

    // If no tokens, return all entries
    if (input_text == L"") {
	Options_Indexes_Searched.resize(Options_All_.size());
	std::iota(Options_Indexes_Searched.begin(), Options_Indexes_Searched.end(), 0);
	return true;
    }

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<size_t> exactMatches;
    std::vector<size_t> prefixMatches;
    std::vector<size_t> substringMatches;

    // Iterate through each item in the search array
    for (size_t i = 0; i < Options_All_.size(); i++)
    {
	// Check if all of the input tokens can be found in the item's text
	bool allTokensFound = true;
	for (const std::wstring& token : input_tokens)
	{
	    if (Options_All_[i].find(token) == std::wstring::npos)
	    {
		allTokensFound = false;
		break;
	    }
	}

	// If all of the input tokens were found, determine if the item's text is an exact match,
	// a prefix match, or a substring match, and add the item to the appropriate vector
	if (allTokensFound)
	{
	    if (input_text == Options_All_[i])
	    {
		exactMatches.push_back(i);
	    }
	    else if (Options_All_[i].find(input_tokens[0]) == 0)
	    {
		prefixMatches.push_back(i);
	    }
	    else
	    {
		substringMatches.push_back(i);
	    }
	}
    }

    // Concatenate the exact, prefix, and substring matches vectors and add them to the output array
    Options_Indexes_Searched.insert(Options_Indexes_Searched.end(), exactMatches.begin(), exactMatches.end());
    Options_Indexes_Searched.insert(Options_Indexes_Searched.end(), prefixMatches.begin(), prefixMatches.end());
    Options_Indexes_Searched.insert(Options_Indexes_Searched.end(), substringMatches.begin(), substringMatches.end());

    return true;
}
