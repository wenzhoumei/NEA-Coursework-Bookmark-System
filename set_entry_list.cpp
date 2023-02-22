#include <boost/algorithm/string.hpp>
#include "set_entry_list.hpp"

void SetEntryList::Search()
{
    if (!search_on_) return;
    searched_.clear();

    std::vector<std::wstring> input_tokens;
    boost::split(input_tokens, input_text_, boost::is_any_of(L" \t\n\r."), boost::token_compress_on);

    // If no tokens, return all entries
    if (input_tokens.size() == 0) {
	SetSearchedToEntries_();
        return;
    }

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<std::wstring> exactMatches;
    std::vector<std::wstring> prefixMatches;
    std::vector<std::wstring> substringMatches;

    // Iterate through each item in the search array
    for (const auto& [key, val]: entry_dict_)
    {
	// Check if all of the input tokens can be found in the item's text
	bool allTokensFound = true;
	for (const std::wstring& token : input_tokens)
	{
	    if (key.find(token) == std::wstring::npos)
	    {
		allTokensFound = false;
		break;
	    }
	}

	// If all of the input tokens were found, determine if the item's text is an exact match,
	// a prefix match, or a substring match, and add the item to the appropriate vector
	if (allTokensFound)
	{
	    if (input_text_ == key)
	    {
		exactMatches.push_back(key);
	    }
	    else if (key.find(input_tokens[0]) == 0)
	    {
		prefixMatches.push_back(key);
	    }
	    else
	    {
		substringMatches.push_back(key);
	    }
	}
    }

    // Concatenate the exact, prefix, and substring matches vectors and add them to the output array
    searched_.insert(searched_.end(), exactMatches.begin(), exactMatches.end());
    searched_.insert(searched_.end(), prefixMatches.begin(), prefixMatches.end());
    searched_.insert(searched_.end(), substringMatches.begin(), substringMatches.end());

    NeedsUpdate.Menu = true;
    SetSelPos_(0);
}
