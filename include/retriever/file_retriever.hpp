#pragma once

#include <filesystem>

#include "retriever.hpp"

/**
 * @brief Buffer store for IdentifierExtension_To_Action_
 */
class FileRetriever: public Retriever {
public:
    FileRetriever(const std::filesystem::path& file_path);
    bool Load() override;

private:
    std::filesystem::path file_path_;
};
