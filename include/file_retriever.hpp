#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "retriever.hpp"

class FileRetriever: public Retriever {
public:
    FileRetriever(std::filesystem::path file_path): file_path_(file_path) {}
    bool Load() override {
	if (!std::filesystem::exists(file_path_)) {
	    std::cerr << "Error: File " << file_path_ << " does not exist." << std::endl;
	    return false;
	}

	std::ifstream file_stream(file_path_);
	if (!file_stream.is_open()) {
	    std::cerr << "Error: Could not open file " << file_path_ << std::endl;
	    return false;
	}

	std::string line;

	while (std::getline(file_stream, line)) {
	    data_.push_back(line);
	}

	file_stream.close();

	return true;
    }

private:
    std::filesystem::path file_path_;
};
