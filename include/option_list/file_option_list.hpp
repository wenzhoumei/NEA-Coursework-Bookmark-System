#pragma once
#include "option_list.hpp"
#include "retriever/file_retriever.hpp"

/**
 * @brief Connects to option list to file, where each line represents a option string
 */
class FileOptionList: public OptionList {
protected:
    std::unique_ptr<FileRetriever> File_Retriever_ = nullptr;

    /**
     * @brief Flushes vector to file returns true if successful else false
     */
    virtual bool Flush_() = 0;

    /**
     * @brief Sets given location to absolute path to allow for travelling through MenuData::History
     */
    void SetLocationPath();
public:
    using OptionList::OptionList;

    /**
     * @brief Writes to file after OptionList::Add is called
     */
    ModifyStatus Add(const std::wstring& option_string) override;

    /**
     * @brief Writes to file after OptionList::Remove is called
     */
    ModifyStatus Remove(size_t pos) override;

    /**
     * @brief Writes to file after OptionList::Update is called
     */
    ModifyStatus Update(size_t pos, const std::wstring& new_option_string) override;

    /**
     * @brief Writes to file each time OptionList::Update is called
     */
    ModifyStatus Insert(size_t pos, const std::wstring& option_string) override;
};
