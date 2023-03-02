#include "config_retriever.hpp"

bool ConfigRetriever::LoadDirectory(const std::filesystem::path& dir_path, std::set<std::string>& dest) {
    try {
	// Iterate through the files in the directory and add their names to the Scripts set
	for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
	    if (entry.is_regular_file()) {
		dest.insert(entry.path().filename().string());
	    }
	}
	return true;
    } catch (const std::filesystem::filesystem_error& e) {
	// An error occurred while accessing the directory, return false
	return false;
    }
}

bool ConfigRetriever::LoadScripts() {
    std::filesystem::path dir_path = config_directory_.GetScriptsDirectoryPath();
    return LoadDirectory(dir_path, Scripts);
}

bool ConfigRetriever::LoadOptionLists() {
    std::filesystem::path dir_path = config_directory_.GetOptionListsDirectoryPath();
    return LoadDirectory(dir_path, OptionLists);
}

bool ConfigRetriever::LoadIdentifierExtensions() {
    std::filesystem::path list_path = config_directory_.GetIdentifierExtensionsFilePath();
    try {
	// Open the file
	std::ifstream file(list_path);
	if (!file.is_open()) {
	    // The file couldn't be opened, return false
	    return false;
	}

	// Read the file line by line
	std::string line;
	while (std::getline(file, line)) {
	    // Split the line into two parts: the file extension and the action
	    std::istringstream iss(line);
	    std::string extension, action;
	    if (std::getline(iss, extension, ',')) {
		std::getline(iss, action);
		IdentifierExtensions[extension] = action;
	    }
	}

	return true;
    } catch (const std::filesystem::filesystem_error& e) {
	// An error occurred while accessing the file, return false
	return false;
    }
}
