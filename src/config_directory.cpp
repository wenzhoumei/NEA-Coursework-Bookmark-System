#include <filesystem>
#include "retriever/directory_retriever.hpp"
#include "retriever/file_retriever.hpp"

#include "log.hpp"
#include "config_directory.hpp"

void ConfigDirectory::Initialize(const std::filesystem::path& directory_path) {
    Config_Directory_Path_ = directory_path;

    // Exits if fails
    CheckDirectory_(directory_path);
    CheckFile_(GetLogFilePath());
    CheckFile_(GetHistoryFilePath());
    CheckFile_(GetWorkspaceFilePath());
    CheckDirectory_(GetScriptsDirectoryPath());
    CheckDirectory_(GetOptionListsDirectoryPath());
    CheckFile_(GetIdentifierExtensionsFilePath());

    Scripts_Retriever = std::make_unique<DirectoryRetriever>(DirectoryRetriever(GetScriptsDirectoryPath()));
    IdentifierExtension_To_Action_Retriever = std::make_unique<FileRetriever>(FileRetriever(GetIdentifierExtensionsFilePath()));

    if (!Scripts_Retriever->Load()) {
	my::log.Error(5) << "Scripts failed to load";
    }

    if (!IdentifierExtension_To_Action_Retriever->Load()) {
	my::log.Error(5) << "Identifier extensions failed to load";
    }
}

std::filesystem::path ConfigDirectory::GetPath() const {
    return Config_Directory_Path_;
}

std::filesystem::path ConfigDirectory::GetLogFilePath() const {
    return Config_Directory_Path_/LOG_FILE_NAME_;
}

std::filesystem::path ConfigDirectory::GetScriptsDirectoryPath() const {
    return Config_Directory_Path_/SCRIPTS_DIRECTORY_NAME_;
}

std::filesystem::path ConfigDirectory::GetOptionListsDirectoryPath() const {
    return Config_Directory_Path_/OPTION_LISTS_DIR_NAME_;
}

std::filesystem::path ConfigDirectory::GetIdentifierExtensionsFilePath() const {
    return Config_Directory_Path_/IDENTIFIER_EXTENSIONS_FILE_NAME_;
}

std::filesystem::path ConfigDirectory::GetHistoryFilePath() const {
    return Config_Directory_Path_/HISTORY_FILE_NAME_;
}

std::filesystem::path ConfigDirectory::GetWorkspaceFilePath() const {
    return Config_Directory_Path_/WORKSPACE_FILE_NAME_;
}

std::wstring ConfigDirectory::GetWorkspaceFileContents() const {
    // Get the path to the workspace file
    std::filesystem::path workspaceFilePath = GetWorkspaceFilePath();

    // Read the contents of the file
    std::wifstream file(workspaceFilePath);
    if (file.is_open()) {
        // Read the file contents without newline characters
        std::wstring content;
        std::getline(file, content, L'\0');
	file.close();

        return content;
    }
    return L"";
}

bool ConfigDirectory::SetWorkspaceFileContents(const std::wstring& option_string) const {
    // Get the path to the workspace file
    std::filesystem::path workspaceFilePath = GetWorkspaceFilePath();

    // Write the new contents to the file
    std::wofstream file(workspaceFilePath);
    if (file.is_open()) {
        file << option_string;
	file.close();
        return true;
    }

    return false;
}

void ConfigDirectory::CheckDirectory_(const std::filesystem::path& directory_path) {
    if (!std::filesystem::exists(directory_path)) {
	my::log.Error(1) << "The specified directory does not exist: " << directory_path;
    } else if (!std::filesystem::is_directory(directory_path)) {
	my::log.Error(2) << "The specified path is not a directory: " << directory_path;
    }
}

void ConfigDirectory::CheckFile_(const std::filesystem::path& file_path) {
    if (!std::filesystem::exists(file_path)) {
	my::log.Error(1) << "Error: The specified file does not exist: " << file_path;
    } else if (!std::filesystem::is_regular_file(file_path)) {
	my::log.Error(2) << "Error: The specified path is not a regular file: " << file_path;
    }
}
