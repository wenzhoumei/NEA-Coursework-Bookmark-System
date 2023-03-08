#include "option_list.hpp"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <numeric>

void OptionList::Search(const std::wstring& input_text)
{
    searched_.clear();

    std::vector<std::wstring> input_tokens;
    boost::split(input_tokens, input_text, boost::is_any_of(L" \t.-"), boost::token_compress_on);

    // If no tokens, return all entries
    if (input_tokens.size() == 0) {
        searched_.resize(options_.size());
        std::iota(searched_.begin(), searched_.end(), 0);
        return;
    }

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<size_t> exactMatches;
    std::vector<size_t> prefixMatches;
    std::vector<size_t> substringMatches;

    // Iterate through each item in the search array
    for (size_t i = 0; i < options_.size(); i++)
    {
	// Check if all of the input tokens can be found in the item's text
	bool allTokensFound = true;
	for (const std::wstring& token : input_tokens)
	{
	    if (options_[i].find(token) == std::wstring::npos)
	    {
		allTokensFound = false;
		break;
	    }
	}

	// If all of the input tokens were found, determine if the item's text is an exact match,
	// a prefix match, or a substring match, and add the item to the appropriate vector
	if (allTokensFound)
	{
	    if (input_text == options_[i])
	    {
		exactMatches.push_back(i);
	    }
	    else if (options_[i].find(input_tokens[0]) == 0)
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
    searched_.insert(searched_.end(), exactMatches.begin(), exactMatches.end());
    searched_.insert(searched_.end(), prefixMatches.begin(), prefixMatches.end());
    searched_.insert(searched_.end(), substringMatches.begin(), substringMatches.end());
}
