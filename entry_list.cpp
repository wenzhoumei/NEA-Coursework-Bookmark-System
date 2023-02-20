#include "entry_list.hpp"

std::wstring EntryList::SplitByLast_(std::wstring& str, wchar_t del) {
    size_t last_del_pos = str.find_last_of(del);
    if (last_del_pos == std::wstring::npos) {
	// Delimiter not found, return empty string
	return L"";
    } else {
	// Split string into two parts
	std::wstring first_part = str.substr(0, last_del_pos);
	std::wstring end_part = str.substr(last_del_pos + 1);
	// Update input string to be the first part
	str = first_part;
	// Return the end part
	return end_part;
    }
}

std::wstring EntryList::SplitByFirst_(std::wstring& str, wchar_t del) {
    size_t first_del_pos = str.find(del);
    if (first_del_pos == std::wstring::npos) {
	// Delimiter not found, return empty string
	return L"";
    } else {
	// Split string into two parts
	std::wstring first_part = str.substr(0, first_del_pos);
	std::wstring end_part = str.substr(first_del_pos + 1);
	// Update input string to be the end part
	str = end_part;
	// Return the first part
	return first_part;
    }
}

std::wstring EntryList::SplitByIndex_(std::wstring& str, size_t del_pos) {
    // Split string into two parts
    std::wstring first_part = str.substr(0, del_pos);
    std::wstring end_part = str.substr(del_pos + 1);
    // Update input string to be the first part
    str = first_part;
    // Return the end part
    return end_part;
}

/*
std::unique_ptr<Entry> EntryList::ProcessEntry(std::unique_ptr<Entry> entry) {
    std::wstring default_script = ""; // TMP

    std::wstring name = entry->GetName();
    std::wstring data = entry->GetData();

    std::wstring str_copy = std::string(str);
    std::wstring data = SplitByFirst_(str_copy, '\t');

    std::wstring script = default_script;

    enum del { LIST = '@', SCRIPT = '|' } entry_delimiter = SCRIPT;

    size_t pos = str_copy.find_last_of("|@");
    if (pos == std::wstring::npos) {
	script = default_script;
    } else if (data[pos] == del::SCRIPT) {
	entry_delimiter = del::SCRIPT; 
	script = SplitByIndex_(str_copy, pos);
    } else if (data[pos] == del::LIST) {
	entry_delimiter = del::LIST;
	script = SplitByIndex_(str_copy, pos);
    }

    if (!script_list.Contains(CreateEntry(script))) {
	str_copy += (char)entry_delimiter + script;
	std::wstring extension = SplitByLast_(str_copy, '.');
	std::unique_ptr<Entry> script_corresponding_to_extension = extension_list.GetEntryAt(CreateEntry(extension));
	if (script_corresponding_to_extension == nullptr) {
	    script = default_script;
	} else {
	    script = script_corresponding_to_extension->GetName();
	}
    }

    if (entry_delimiter == LIST) {
    }

    return std::make_unique<DefaultEntry>(str, script, data);
}
*/
