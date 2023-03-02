#pragma once

#include <filesystem>
#include <unordered_map>
#include <fstream>

#include <unordered_map>
#include "config_directory.hpp"

class ConfigWriter
{
public:
    ConfigWriter(ConfigDirectory& config_directory)
	: config_directory_(config_directory) {}

    ConfigDirectory& config_directory_;

    bool RenameScript(const std::string& script_name);
    bool CreateScript(const std::string& script_name);
    bool RemoveScript(const std::string& script_name);

    bool UpdateIdentifierExtensionSettings(const std::unordered_map<std::string, std::string>& script_name);
};
