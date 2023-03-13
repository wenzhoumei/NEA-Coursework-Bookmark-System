#pragma once
#include "option_list.hpp"
#include "retriever/file_retriever.hpp"

class FileOptionList: public OptionList {
protected:
    std::unique_ptr<FileRetriever> File_Retriever_ = nullptr;

    virtual bool Flush_() = 0; // Writes to backend
			       // Returns true if successful else false

    void SetLocationPath();
public:
    using OptionList::OptionList;

    ModifyStatus Add(const std::wstring& option_string) override;
    ModifyStatus Remove(size_t pos) override;
    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;
    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;
};
