#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "retriever.hpp"

/**
 * @brief Buffer store for Parser::Scripts_
 */
class DirectoryRetriever: public Retriever {
public:
    DirectoryRetriever(const std::filesystem::path& directory_path);

    /**
     * @brief Loads directory names into a data_ set
     */
    bool Load() override;

private:
    std::filesystem::path directory_path_;
};
