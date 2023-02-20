#include "vector_entry_list.hpp"
#include <iterator>

void VectorEntryList::Search(const std::string& search)
{
    searched_.clear();

    // Split the input text into tokens
    std::istringstream iss(search);
    std::vector<std::string> inputTokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    // If no tokens, return all entries
    if (inputTokens.size() == 0) {
	SetSearchedToEntries_();
        return;
    }

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<int> exactMatches;
    std::vector<int> prefixMatches;
    std::vector<int> substringMatches;

    // Iterate through each item in the search array
    for (int i = 0; i < entries_.size(); i++)
    {
	// Check if all of the input tokens can be found in the item's text
	bool allTokensFound = true;
	for (const std::string& token : inputTokens)
	{
	    if (entries_[i].find(token) == std::string::npos)
	    {
		allTokensFound = false;
		break;
	    }
	}

	// If all of the input tokens were found, determine if the item's text is an exact match,
	// a prefix match, or a substring match, and add the item to the appropriate vector
	if (allTokensFound)
	{
	    if (search == entries_[i])
	    {
		exactMatches.push_back(i);
	    }
	    else if (entries_[i].find(inputTokens[0]) == 0)
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
