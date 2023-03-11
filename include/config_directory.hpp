#pragma once

#include <filesystem>
#include "retriever/directory_retriever.hpp"
#include "retriever/file_retriever.hpp"

#include "log.hpp"

class ConfigDirectory {
    const std::filesystem::path scripts_directory_name_ = "SCRIPTS";
    const std::filesystem::path identifier_extensions_file_name_ = "IDENTIFIER_EXTENSIONS";
    const std::filesystem::path option_lists_file_name_ = "OPTION_LISTS";
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

    void Initialize(const std::filesystem::path& directory_path) {
	config_directory_path_ = directory_path;

	Log& log = Log::Instance();

	// Exits if fails
	CheckDirectory(directory_path);
	CheckFile(GetLogFilePath());
	CheckDirectory(GetScriptsDirectoryPath());
	CheckDirectory(GetOptionListsDirectoryPath());
	CheckFile(GetIdentifierExtensionsFilePath());

	Scripts_Retriever = std::make_unique<DirectoryRetriever>(DirectoryRetriever(GetScriptsDirectoryPath()));
	IdentifierExtension_To_Action_Retriever = std::make_unique<FileRetriever>(FileRetriever(GetIdentifierExtensionsFilePath()));

	if (!Scripts_Retriever->Load()) {
	    log.Error(5) << "Scripts failed to load";
	}

	if (!IdentifierExtension_To_Action_Retriever->Load()) {
	    log.Error(5) << "Identifier extensions failed to load";
	}
    }

    std::unique_ptr<DirectoryRetriever> Scripts_Retriever = nullptr;
    std::unique_ptr<FileRetriever> IdentifierExtension_To_Action_Retriever = nullptr;

    std::filesystem::path GetLogFilePath() const {
	return config_directory_path_/log_file_name_;
    }

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
private:
    static void CheckDirectory(std::filesystem::path directory_path) {
	Log& log = Log::Instance();

	if (!std::filesystem::exists(directory_path)) {
	    log.Error(1) << "The specified directory does not exist: " << directory_path;
	} else if (!std::filesystem::is_directory(directory_path)) {
	    log.Error(2) << "The specified path is not a directory: " << directory_path;
	}
    }

    static void CheckFile(std::filesystem::path file_path) {
	Log& log = Log::Instance();

	if (!std::filesystem::exists(file_path)) {
	    log.Error(1) << "Error: The specified file does not exist: " << file_path;
	} else if (!std::filesystem::is_regular_file(file_path)) {
	    log.Error(2) << "Error: The specified path is not a regular file: " << file_path;
	}
    }
};
