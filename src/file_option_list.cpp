#include "option_list/file_option_list.hpp"
#include "config_directory.hpp"

void FileOptionList::SetLocationPath() {
    std::filesystem::path parent_path = std::filesystem::path(Location_).parent_path();
    if (!parent_path.empty() && std::filesystem::exists(parent_path) && std::filesystem::is_directory(parent_path)) {
	// If user is trying to specify a file load it
	Location_ = std::filesystem::path(Location_).wstring();
    } else {
	// In default option list directory
	Location_ = (ConfigDirectory::Instance().GetOptionListsDirectoryPath()/std::filesystem::path(Location_)).wstring();
    }
}

OptionList::ModifyStatus FileOptionList::Add(const std::wstring& option_string) {
    ModifyStatus m_s = OptionList::Add(option_string);
    m_s.BackendError = m_s.BackendError || !Flush_();
    return m_s;
}

OptionList::ModifyStatus FileOptionList::Remove(size_t pos)  {
    ModifyStatus m_s = OptionList::Remove(pos);
    m_s.BackendError = m_s.BackendError || !Flush_();
    return m_s;
}

OptionList::ModifyStatus FileOptionList::Update(size_t pos, const std::wstring& new_option_string)  {
    ModifyStatus m_s = OptionList::Update(pos, new_option_string);
    m_s.BackendError = m_s.BackendError || !Flush_();
    return m_s;
}

OptionList::ModifyStatus FileOptionList::Insert(size_t pos, const std::wstring& option_string)  {
    ModifyStatus m_s = OptionList::Insert(pos, option_string);
    m_s.BackendError = m_s.BackendError || !Flush_();
    return m_s;
}
