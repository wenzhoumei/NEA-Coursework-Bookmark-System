#pragma once
#include <filesystem>

class ConfigDirectory {
    ConfigDirectory() {}; // Private constructor to prevent instantiation
    ConfigDirectory(const ConfigDirectory&) = delete; // Delete copy constructor
    ConfigDirectory& operator=(const ConfigDirectory&) = delete; // Delete assignment operato

    const std::filesystem::path scripts_directory_name_ = "SCRIPTS";
    const std::filesystem::path option_lists_directory_name_ = "OPTION_LISTS";

    const std::filesystem::path identifier_extensions_file_name_ = "FILE_EXTENSIONS";

    std::filesystem::path config_directory_path_;
public:
    static ConfigDirectory& Instance()
    {
	static ConfigDirectory INSTANCE;
	return INSTANCE;
    }

    bool SetPath(const std::filesystem::path& directory_path) {
	config_directory_path_ = directory_path;
	return std::filesystem::is_directory(directory_path);
    }

    std::filesystem::path GetScriptsDirectoryPath() {
	return config_directory_path_/scripts_directory_name_;
    }

    std::filesystem::path GetOptionListsDirectoryPath() {
	return config_directory_path_/scripts_directory_name_;
    }

    std::filesystem::path GetIdentifierExtensionsFilePath() {
	return config_directory_path_/scripts_directory_name_/identifier_extensions_file_name_;
    }
};
