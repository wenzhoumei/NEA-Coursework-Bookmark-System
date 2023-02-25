#pragma once

#include <filesystem>
#include <unordered_map>
#include <fstream>

class Settings
{
private:
    Settings() {}; // Private constructor to prevent instantiation
    Settings(const Settings&) = delete; // Delete copy constructor
    Settings& operator=(const Settings&) = delete; // Delete assignment operato

public:
    static const struct {
	const char Data = ',';
	const char Script = '|';
	const char FileExtension = '.';
    } Delimiter;

    static const struct {
	const std::filesystem::path Scripts = "scripts";
	const std::filesystem::path OptionLists = "option_lists";
    } DirectoryName;

    static const struct {
	const std::filesystem::path Scripts = "SCRIPTS";
	const std::filesystem::path FileExtensions = "FILE_EXTENSIONS";
    } OptionListName;

    static const struct {
	const std::filesystem::path Default = "DEFAULT";
    } ScriptName;

    static Settings& Instance()
    {
	static Settings INSTANCE;
	return INSTANCE;
    }

    std::filesystem::path DirectoryPath;
    bool Initialize(const std::filesystem::path& directory_path) {
	DirectoryPath = directory_path;
	return std::filesystem::is_directory(directory_path);
    }

    std::unordered_map<std::string, std::string> Scripts;
    bool LoadScriptsNames() {
	std::filesystem::path scripts_dir_path = DirectoryPath/DirectoryName.Scripts;
	if (!std::filesystem::is_directory(scripts_dir_path)) { return false; }
	Scripts = LoadScriptTypes(
	if (!Scripts.contains(ScriptName.Default)) { return false; }
	if (Scripts.size() == 0) { return false; };
	return true;
    }

    bool LoadScriptTypes() {
	std::filesystem::path list_path = DirectoryPath/DirectoryName.OptionLists/OptionListName.Scripts;
	if (!EnsureFileExists(list_path)) return false;

	std::ifstream file(list_path);
	std::unordered_map<std::string, std::string> config_map = LoadCommaSeparatedConfigFile_(file);
	file.close();

	return true;
    }

    std::unordered_map<std::string, std::string> FileExtensions;
    bool LoadFileExtensions() {
	std::filesystem::path list_path = DirectoryPath/DirectoryName.OptionLists/OptionListName.FileExtensions;
	if (!EnsureFileExists(list_path)) return false;

	std::ifstream file(list_path);
	std::unordered_map<std::string, std::string> config_map = LoadCommaSeparatedConfigFile_(file);
	file.close();

	return true;
    }

private:
    std::unordered_map<std::string, std::string> LoadDirectory(std::filesystem::path dir_path, std::string default_value) {
	std::unordered_map<std::string, std::string> ret_map;
	for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
	    if (entry.is_regular_file()) {
		std::wstring file_name = entry.path().filename().wstring();
		ret_map.emplace(file_name, default_value);
	    }
	}
	return ret_map;
    }

    std::unordered_map<std::string, std::string> LoadCommaSeparatedConfigFile_(std::ifstream& file) {
	std::unordered_map<std::string, std::string> ret_map;

	// Load all "names" into keys of 
	std::string line;
	while (std::getline(file, line)) {
	    int i_del = line.find_first_of(Settings::Delimiter.Data);
	    std::string name = line.substr(0, i_del);
	    std::string data = line.substr(i_del + 1);
	    Scripts[name] = data;
	}

	return ret_map;
    }

    bool EnsureFileExists(const std::filesystem::path& file_path) {
	std::ifstream file(file_path);
	if (file.good()) {
	    return true;  // File exists
	}
	// If file doesn't exist, create it
	std::ofstream new_file(file_path);
	if (!new_file.good()) {
	    return false;
	}

	return true;
    }
};
