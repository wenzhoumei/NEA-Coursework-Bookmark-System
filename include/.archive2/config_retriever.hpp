#pragma once

#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <functional>
#include <set>
#include "config_directory.hpp"

class ConfigRetriever
{
public:
    ConfigRetriever(ConfigDirectory& config_directory)
	:config_directory_(config_directory) {};

    std::set<std::string> Scripts;
    std::unordered_map<std::string, std::string> IdentifierExtensions;

    /**
     * @brief Loads file names from DirectoryPath/Directory.Scripts directory into Scripts set
     * 
     * This function uses the C++ standard library's `std::filesystem` library to iterate through
     * the files in the directory and add their names to the `Scripts` set.
     * 
     * @return true if the operation was successful, false otherwise
     */
    bool LoadScripts();

    /**
     * @brief Loads a file containing file extensions and their corresponding actions into the IdentifierExtensions map
     *
     * This function loads a file located at DirectoryPath/Directory.OptionLists/OptionList.Extensions.
     * The file is expected to have one entry per line, with the file extension and its corresponding action separated
     * by first comma (`,`). The extension should be the key in the map and the action should be the value.
     *
     * Example file:
     *
     *    mp4,|play_video
     *    list,@menu
     *    txt,|edit_text
     *
     * @return true if the operation was successful, false otherwise
     */
    bool LoadIdentifierExtensions();
private:
    ConfigDirectory& config_directory_;
    bool LoadDirectory(const std::filesystem::path& dir_path, std::set<std::string>& dest);
};
