#include "option_list/scripts_directory_option_list.hpp"
#include "config_directory.hpp"
#include "parser.hpp"
#include "log.hpp"

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

	if (std::filesystem::equivalent(std::filesystem::path(Location_), ConfigDirectory::Instance().GetScriptsDirectoryPath())) { Parser::Instance().LoadScripts(); }

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

	if (std::filesystem::equivalent(std::filesystem::path(Location_), ConfigDirectory::Instance().GetScriptsDirectoryPath())) { Parser::Instance().LoadScripts(); }

	return m_s;
    }

OptionList::ModifyStatus ScriptsDirectoryOptionList::Remove(size_t pos) {
	if (std::filesystem::equivalent(std::filesystem::path(Location_), ConfigDirectory::Instance().GetScriptsDirectoryPath()) && Action_Out_Of_Here_.substr(1) == NameAt(pos)) {
	    my::log.Warning() << "Can't remove default action for this option list" << std::endl;
	    return { false, false };
	} else {
	    ModifyStatus m_s = EditableDirectoryOptionList::Remove(pos);
	    Parser::Instance().LoadScripts();
	    return m_s;
	}
}

OptionList::ModifyStatus ScriptsDirectoryOptionList::Update(size_t pos, const std::wstring& new_option_string) {
	if (std::filesystem::equivalent(std::filesystem::path(Location_), ConfigDirectory::Instance().GetScriptsDirectoryPath()) && Action_Out_Of_Here_.substr(1) == NameAt(pos)) {
	    my::log.Warning() << "Can't remove default action for this option list" << std::endl;
	    return { false, false };
	} else {
	    ModifyStatus m_s = EditableDirectoryOptionList::Update(pos, new_option_string);
	    Parser::Instance().LoadScripts();
	    return m_s;
	}
}
