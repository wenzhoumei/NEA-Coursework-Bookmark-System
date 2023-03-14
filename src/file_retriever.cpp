#include "retriever/file_retriever.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

FileRetriever::FileRetriever(const std::filesystem::path& file_path): file_path_(file_path) {}

bool FileRetriever::Load() {
    data_.clear();
    if (!std::filesystem::exists(file_path_)) {
	std::ofstream create_file(file_path_);
	if (!create_file.is_open()) {
	    return false;
	}
	create_file.close();
    }

    std::wifstream file_stream(file_path_);
    if (!file_stream.is_open()) {
	return false;
    }

    std::wstring line;

    while (std::getline(file_stream, line)) {
	data_.push_back(line);
    }

    file_stream.close();
    return true;
}
