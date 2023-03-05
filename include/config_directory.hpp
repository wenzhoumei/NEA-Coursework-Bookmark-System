#pragma once

#include <filesystem>
#include "directory_retriever.hpp"
#include "file_retriever.hpp"

class ConfigDirectory {
    const std::filesystem::path scripts_directory_name_ = "SCRIPTS";

    const std::filesystem::path identifier_extensions_file_name_ = "IDENTIFIER_EXTENSIONS";

    const std::filesystem::path option_lists_file_name_ = "OPTION_LISTS";

    std::filesystem::path config_directory_path_;
public:
    bool SetPath(const std::filesystem::path& directory_path) {
	config_directory_path_ = directory_path;

	if (!std::filesystem::exists(directory_path)) {
	    std::cerr << "Error: The specified directory does not exist: " << directory_path << std::endl;
	    return false;
	} else if (!std::filesystem::is_directory(directory_path)) {
	    std::cerr << "Error: The specified path is not a directory: " << directory_path << std::endl;
	    return false;
	} else if (!std::filesystem::exists(GetScriptsDirectoryPath())) {
	    std::cerr << "Error: The 'scripts' directory does not exist: " << GetScriptsDirectoryPath() << std::endl;
	    return false;
	} else if (!std::filesystem::is_directory(GetScriptsDirectoryPath())) {
	    std::cerr << "Error: The 'scripts' path is not a directory: " << GetScriptsDirectoryPath() << std::endl;
	    return false;
	} else if (!std::filesystem::exists(GetOptionListsDirectoryPath())) {
	    std::cerr << "Error: The 'option_lists' directory does not exist: " << GetOptionListsDirectoryPath() << std::endl;
	    return false;
	} else if (!std::filesystem::is_directory(GetOptionListsDirectoryPath())) {
	    std::cerr << "Error: The 'option_lists' path is not a directory: " << GetOptionListsDirectoryPath() << std::endl;
	    return false;
	} else if (!std::filesystem::exists(GetIdentifierExtensionsFilePath())) {
	    std::cerr << "Error: The 'identifier_extensions' file does not exist: " << GetIdentifierExtensionsFilePath() << std::endl;
	    return false;
	} else if (!std::filesystem::is_regular_file(GetIdentifierExtensionsFilePath())) {
	    std::cerr << "Error: The 'identifier_extensions' path is not a regular file: " << GetIdentifierExtensionsFilePath() << std::endl;
	    return false;
	} else {
	    Scripts_Retriever = std::make_unique<DirectoryRetriever>(DirectoryRetriever(GetScriptsDirectoryPath()));
	    IdentifierExtension_To_Action_Retriever = std::make_unique<FileRetriever>(FileRetriever(GetIdentifierExtensionsFilePath()));
	    return true;
	}
    }

    std::unique_ptr<DirectoryRetriever> Scripts_Retriever = nullptr;
    std::unique_ptr<FileRetriever> IdentifierExtension_To_Action_Retriever = nullptr;

    std::filesystem::path GetScriptsDirectoryPath() const {
	return config_directory_path_/scripts_directory_name_;
    }

    std::filesystem::path GetOptionListsDirectoryPath() const {
	return config_directory_path_/option_lists_file_name_;
    }

    std::filesystem::path GetScriptPath(std::filesystem::path script_name) const {
	return GetScriptsDirectoryPath()/script_name;
    }


    std::filesystem::path GetOptionListPath(std::filesystem::path option_list_name) const {
	return config_directory_path_/option_lists_file_name_/option_list_name;
    }

    std::filesystem::path GetIdentifierExtensionsFilePath() const {
	return config_directory_path_/identifier_extensions_file_name_;
    }
};
