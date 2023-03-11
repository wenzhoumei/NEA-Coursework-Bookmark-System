#pragma once
#include "option_list.hpp"
#include "retriever/file_retriever.hpp"

class FileOptionList: public OptionList {
protected:
    std::unique_ptr<FileRetriever> File_Retriever_ = nullptr;

    virtual bool Flush_() = 0; // Writes to backend
			       // Returns true if successful else false

public:
    using OptionList::OptionList;

    ModifyStatus Add(const std::wstring& option_string) override {
	ModifyStatus m_s = OptionList::Add(option_string);
	m_s.BackendError = m_s.BackendError || !Flush_();
	return m_s;
    }

    ModifyStatus Remove(size_t pos) override {
	ModifyStatus m_s = OptionList::Remove(pos);
	m_s.BackendError = m_s.BackendError || !Flush_();
	return m_s;
    }

    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override {
	ModifyStatus m_s = OptionList::Update(pos, new_option_string);
	m_s.BackendError = m_s.BackendError || !Flush_();
	return m_s;
    }

    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override {
	ModifyStatus m_s = OptionList::Insert(pos, option_string);
	m_s.BackendError = m_s.BackendError || !Flush_();
	return m_s;
    }
};
