#include "option_list/scripts_directory_option_list.hpp"
#include "parser.hpp"

OptionList::ModifyStatus ScriptsDirectoryOptionList::Add(const std::wstring& option_string) {
	ModifyStatus m_s = EditableDirectoryOptionList::Add(option_string);
	if (m_s.Modified && !m_s.BackendError) {
	    std::filesystem::path path(Location_);
	    path /= option_string;
	    // Change file permissions to executable
	    std::filesystem::permissions(path,
				      std::filesystem::perms::owner_exec |
				      std::filesystem::perms::group_exec |
				      std::filesystem::perms::others_exec,
				      std::filesystem::perm_options::add);
	}

	return m_s;
    }

OptionList::ModifyStatus ScriptsDirectoryOptionList::Insert(size_t pos, const std::wstring& option_string) {
	ModifyStatus m_s = EditableDirectoryOptionList::Insert(pos, option_string);
	if (m_s.Modified && !m_s.BackendError) {
	    std::filesystem::path path(Location_);
	    path /= option_string;
	    // Change file permissions to executable
	    std::filesystem::permissions(path,
				      std::filesystem::perms::owner_exec |
				      std::filesystem::perms::group_exec |
				      std::filesystem::perms::others_exec,
				      std::filesystem::perm_options::add);
	}

	return m_s;
    }

OptionList::ModifyStatus ScriptsDirectoryOptionList::Remove(size_t pos) {
	ModifyStatus m_s = EditableDirectoryOptionList::Remove(pos);
	Parser::Instance().LoadScripts();
	return m_s;
}

OptionList::ModifyStatus ScriptsDirectoryOptionList::Update(size_t pos, const std::wstring& new_option_string) {
	ModifyStatus m_s = EditableDirectoryOptionList::Update(pos, new_option_string);
	Parser::Instance().LoadScripts();
	return m_s;
}
