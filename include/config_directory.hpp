#pragma once

#include <filesystem>
#include "retriever/directory_retriever.hpp"
#include "retriever/file_retriever.hpp"

/**
 * @brief Singleton class containing actions relating to the config directory and information about the directory structure of the config directory
 */
class ConfigDirectory {
    const std::filesystem::path SCRIPTS_DIRECTORY_NAME_ = "SCRIPTS";
    const std::filesystem::path IDENTIFIER_EXTENSIONS_FILE_NAME_ = "IDENTIFIER_EXTENSIONS";
    const std::filesystem::path OPTION_LISTS_DIR_NAME_ = "OPTION_LISTS";
    const std::filesystem::path LOG_FILE_NAME_ = "LOG";

    std::filesystem::path Config_Directory_Path_;

    ConfigDirectory() {}; ///> Private constructor to prevent instantiation
    ConfigDirectory(const ConfigDirectory&) = delete; ///> Delete copy constructor
    ConfigDirectory& operator=(const ConfigDirectory&) = delete; //> Delete assignment operato
								 
public:
    /**
     * @brief Access point to only instance of class
     *
     * @return Meyer's singleton
     */
    static ConfigDirectory& Instance()
    {
	static ConfigDirectory INSTANCE;
	return INSTANCE;
    }

    /**
     * @brief Checks if directories and files that should exist exist, tries to load information, and forces exit if there is a failiure
     */
    void Initialize(const std::filesystem::path& directory_path);

    std::unique_ptr<DirectoryRetriever> Scripts_Retriever = nullptr; ///> Should contain all file names of SCRIPTS directory
    std::unique_ptr<FileRetriever> IdentifierExtension_To_Action_Retriever = nullptr; ///> Contains all lines of IDENTIFIER_EXTENSIONS file

    /**
     * @return Path of config directory
     */
    std::filesystem::path GetPath() const;

    /**
     * @return Path of log file
     */
    std::filesystem::path GetLogFilePath() const;

    /**
     * @return Path of scripts directory
     */
    std::filesystem::path GetScriptsDirectoryPath() const;

    /**
     * @return Path of option lists directory
     */
    std::filesystem::path GetOptionListsDirectoryPath() const;

    /**
     * @return Path of identifier extensions file
     */
    std::filesystem::path GetIdentifierExtensionsFilePath() const;

private:
    /**
     * @brief Checks if directory is valid, exits if not
     *
     * @param directory_path Directory path to check
     */
    void CheckDirectory_(const std::filesystem::path& directory_path);
    /**
     * @brief Checks if file is valid, exits if not
     *
     * @param file_path File path to check
     */
    void CheckFile_(const std::filesystem::path& file_path);
};
