#pragma once

#include <filesystem>

class ConfigDirectory {
    const std::filesystem::path scripts_directory_name_ = "SCRIPTS";

    const std::filesystem::path identifier_extensions_file_name_ = "FILE_EXTENSIONS";

    std::filesystem::path config_directory_path_;
public:
    bool SetPath(const std::filesystem::path& directory_path) {
	config_directory_path_ = directory_path;
	return std::filesystem::is_directory(directory_path);
    }

    std::filesystem::path GetScriptsDirectoryPath() const {
	return config_directory_path_/scripts_directory_name_;
    }

    std::filesystem::path GetIdentifierExtensionsFilePath() const {
	return config_directory_path_/identifier_extensions_file_name_;
    }
};
