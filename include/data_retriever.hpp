#pragma once

#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <functional>
#include <set>

class DataRetriever
{
private:
    DataRetriever() {}; // Private constructor to prevent instantiation
    DataRetriever(const DataRetriever&) = delete; // Delete copy constructor
    DataRetriever& operator=(const DataRetriever&) = delete; // Delete assignment operato

public:
    static DataRetriever& Instance()
    {
	static DataRetriever INSTANCE;
	return INSTANCE;
    }

    const struct {
	const std::filesystem::path Scripts = "scripts";
	const std::filesystem::path OptionLists = "option_lists";
    } Directory;

    const struct {
	const std::filesystem::path IdentifierExtensions = "FILE_EXTENSIONS";
    } OptionList;

    const struct {
	const std::filesystem::path None = "";
    } IdentifierExtension;

    std::filesystem::path DirectoryPath;
    std::unordered_map<std::string, std::string> IdentifierExtensions;
    std::set<std::string> Scripts;

    bool Initialize(const std::filesystem::path& directory_path) {
	DirectoryPath = directory_path;
	return std::filesystem::is_directory(directory_path);
    }

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
};
