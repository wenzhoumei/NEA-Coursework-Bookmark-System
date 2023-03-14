#include <filesystem>
#include <fstream>
#include <iostream>

#include "retriever/directory_retriever.hpp"

DirectoryRetriever::DirectoryRetriever(const std::filesystem::path& directory_path): directory_path_(directory_path) {}

bool DirectoryRetriever::Load() {
    // Clear any existing options
    data_.clear();

    if (!std::filesystem::exists(directory_path_)) {
	std::cerr << "Error: Directory " << directory_path_ << " does not exist." << std::endl;
	return false;
    }

    if (!std::filesystem::is_directory(directory_path_)) {
	std::cerr << "Error: Directory " << directory_path_ << " is not a directory." << std::endl;
	return false;
    }

    for (const auto& file : std::filesystem::directory_iterator(directory_path_)) {
	data_.push_back(file.path().filename().wstring());
    }

    return true;
}
