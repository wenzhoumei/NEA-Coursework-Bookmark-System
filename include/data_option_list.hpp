#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include "file_retriever.hpp"
#include "option_list.hpp"

class DataOptionList: public OptionList {
protected:
    std::filesystem::path File_Path_;
    std::unique_ptr<Retriever> Option_Retriever = nullptr;

    bool WriteToFile_() {
        std::wofstream file(File_Path_);

        if (!file) {
            std::cerr << "Error: Unable to open file for writing: " << File_Path_ << std::endl;
            return false;
        }

        for (const std::wstring& entry : options_) {
            file << entry << std::endl;
        }

        return true;
    }

public:
    DataOptionList(std::filesystem::path file_path)
	: File_Path_(file_path), Option_Retriever(std::make_unique<FileRetriever>(FileRetriever(file_path)))
    {
    }


    bool Load() override {
	if (!Option_Retriever->Load()) {
	    std::cerr << "Error: Failed to read file" << std::endl;
	    return false;
	}

	options_ = Option_Retriever->GetData();

	return true;
    }

    bool Add(const std::wstring& option_string) override {
	return OptionList::Add(option_string) && WriteToFile_();
    }
    bool Remove(size_t pos) override {
	return OptionList::Remove(pos) && WriteToFile_();
    }
    bool Update(size_t pos, const std::wstring& new_option_string) override {
	return OptionList::Update(pos, new_option_string) && WriteToFile_();
    }
    bool Insert(size_t pos, const std::wstring& option_string) override {
	return OptionList::Insert(pos, option_string) && WriteToFile_();
    }
};
