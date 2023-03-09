#include "option_list.hpp"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <numeric>

bool OptionList::Search(const std::wstring& input_text)
{
    Options_Indexes_Searched.clear();

    std::vector<std::wstring> input_tokens;
    boost::split(input_tokens, input_text, boost::is_any_of(L" \t.-"), boost::token_compress_on);

    // If no tokens, return all entries
    if (input_tokens.size() == 0) {
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
