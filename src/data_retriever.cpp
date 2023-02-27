#include "data_retriever.hpp"

bool DataRetriever::LoadScripts() {
    std::filesystem::path dir_path = DirectoryPath / Directory.Scripts;
    try {
	// Iterate through the files in the directory and add their names to the Scripts set
	for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
	    if (entry.is_regular_file()) {
		Scripts.insert(entry.path().filename().string());
	    }
	}
	return true;
    } catch (const std::filesystem::filesystem_error& e) {
	// An error occurred while accessing the directory, return false
	return false;
    }
}

bool DataRetriever::LoadIdentifierExtensions() {
    std::filesystem::path list_path = DirectoryPath / Directory.OptionLists / OptionList.IdentifierExtensions;
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
