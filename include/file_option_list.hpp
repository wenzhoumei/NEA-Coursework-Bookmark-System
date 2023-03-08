#pragma once
#include "option_list.hpp"
#include "file_retriever.hpp"

class FileOptionList: public OptionList {
protected:
    std::filesystem::path File_Path_;
    std::unique_ptr<FileRetriever> File_Retriever_ = nullptr;

public:
    virtual bool Flush() = 0;

    bool Add(const std::wstring& option_string) override {
	return OptionList::Add(option_string) && Flush();
    }

    bool Remove(size_t pos) override {
	return OptionList::Remove(pos) && Flush();
    }

    bool Update(size_t pos, const std::wstring& new_option_string) override {
	return OptionList::Update(pos, new_option_string) && Flush();
    }

    bool Insert(size_t pos, const std::wstring& option_string) override {
	return OptionList::Insert(pos, option_string) && Flush();
    }
};
