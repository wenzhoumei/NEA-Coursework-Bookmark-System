#include <algorithm>
#include "option_list/editable_directory_option_list.hpp"
#include "log.hpp"

OptionList::ModifyStatus EditableDirectoryOptionList::Add(const std::wstring& option_string) {
    ModifyStatus m_s = OptionList::Add(option_string);
    if (m_s.Modified) {
	// Create the file
	std::filesystem::path path(Location_);
	path /= option_string;
	std::ofstream file(path);
	if (!file.is_open()) {
	    m_s.BackendError = true;
	}

	file.close();
    }

    return m_s;
}

OptionList::ModifyStatus EditableDirectoryOptionList::Insert(size_t pos, const std::wstring& option_string)  {
    ModifyStatus m_s = OptionList::Insert(pos, option_string);
    if (m_s.Modified) {
	// Create the file
	std::filesystem::path path(Location_);
	path /= option_string;
	std::ofstream file(path);
	if (!file.is_open()) {
	    m_s.BackendError = true;
	}

	file.close();
    }

    return m_s;
}

OptionList::ModifyStatus EditableDirectoryOptionList::Remove(size_t pos)  {
    if (Options_All_.size() == 0) {
	my::log.Write(L"Info: Can't remove, out of range");
	return { false, false };
    }

    std::filesystem::path path = std::filesystem::path(Location_)/std::filesystem::path(Options_All_[pos]);

    if (std::filesystem::is_directory(path)) {
	my::log.Write(L"Info: Can't remove, directory not empty");
	return { false, false };
    }

    ModifyStatus m_s = OptionList::Remove(pos);

    if (m_s.Modified) {

	// Remove file
	bool success = std::filesystem::remove(path);
	if (!success) {
	    m_s.BackendError = true;
	}
    }

    return m_s;
}

OptionList::ModifyStatus EditableDirectoryOptionList::Update(size_t pos, const std::wstring& new_option_string)  {

    if (pos >= Options_All_.size() || Options_All_.size() == 0) { my::log.Write(L"LogicError: Can't update, out of range"); }

    // Check if the new option string already exists
    auto it = std::find(Options_All_.begin(), Options_All_.end(), new_option_string);
    if (it == Options_All_.end()) {
	// If it doesn't exist rename the file
	try {
	    std::filesystem::path old_path(Location_);
	    old_path /= Options_All_[pos];

	    std::filesystem::path new_path(Location_);
	    new_path /= new_option_string;

	    std::filesystem::rename(old_path, new_path);
	} catch (std::filesystem::filesystem_error& e) {
	    return { false, true };
	}

	// Replace the option string at the specified position with the new one
	Options_All_[pos] = new_option_string;
    }

    return { true, false };
}
