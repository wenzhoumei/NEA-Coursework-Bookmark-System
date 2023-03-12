#pragma once

#include <filesystem>
#include "retriever/directory_retriever.hpp"
#include "retriever/file_retriever.hpp"

class ConfigDirectory {
    const std::filesystem::path scripts_directory_name_ = "SCRIPTS";
    const std::filesystem::path identifier_extensions_file_name_ = "IDENTIFIER_EXTENSIONS";
    const std::filesystem::path option_lists_dir_name_ = "OPTION_LISTS";
    const std::filesystem::path log_file_name_ = "LOG";

    std::filesystem::path config_directory_path_;

    ConfigDirectory() {}; // Private constructor to prevent instantiation
    ConfigDirectory(const ConfigDirectory&) = delete; // Delete copy constructor
    ConfigDirectory& operator=(const ConfigDirectory&) = delete; // Delete assignment operato
								 
public:
    static ConfigDirectory& Instance()
    {
	static ConfigDirectory INSTANCE;
	return INSTANCE;
    }

    void Initialize(const std::filesystem::path& directory_path);

    std::unique_ptr<DirectoryRetriever> Scripts_Retriever = nullptr;
    std::unique_ptr<FileRetriever> IdentifierExtension_To_Action_Retriever = nullptr;

    std::filesystem::path GetPath() const;
    std::filesystem::path GetLogFilePath() const;
    std::filesystem::path GetScriptsDirectoryPath() const;
    std::filesystem::path GetOptionListsDirectoryPath() const;

    std::filesystem::path GetIdentifierExtensionsFilePath() const;
private:
    static void CheckDirectory(const std::filesystem::path& directory_path);
    static void CheckFile(const std::filesystem::path& file_path);
};
