#include "option_list.hpp"

class DirectoryOptionList : public OptionList {
public:
    DirectoryOptionList(const std::string& directory_path)
        : directory_path_(directory_path)
    {}

    bool LoadOptions() override;

    bool RemoveOption(const std::string& option_name) override;

    bool InsertOption(size_t pos, const std::string& option_name, const std::string& option_data) override;

    bool UpdateOption(const std::string& option_name, const std::string& option_data) override;

    bool AddOption(const std::string& option_name, const std::string& option_data) override;

private:
    std::string directory_path_;
    
    void WriteOptionsToFile();

    std::string GetFilePath(const std::string& option_name);
};
