#pragma once

#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <functional>
#include <set>

#include "data_retriever.hpp"
#include "action_processor.hpp"

class Settings
{
private:
    Settings(): data_retriever_(DataRetriever::Instance()), action_processor_(ActionProcessor::Instance()) {}; // Private constructor to prevent instantiation
    Settings(const Settings&) = delete; // Delete copy constructor
    Settings& operator=(const Settings&) = delete; // Delete assignment operato
    
    DataRetriever& data_retriever_;
    ActionProcessor& action_processor_;

public:
    static Settings& Instance()
    {
	static Settings INSTANCE;
	return INSTANCE;
    }

    std::set<std::string> Scripts;
    bool LoadActions()
    {
	Scripts = data_retriever_.Scripts;
	for (const auto& script_name: Scripts) {
	    if (ContainsScriptDelimiter || ContainsDataDelimiter) {
		return false;
	    }
	}

	return true;
    }

    std::unordered_map<std::string, std::string> IdentifierExtensions;
    bool LoadIdentifierExtensions()
    {
	for (const auto& [identifier_ext, action_str]: data_retriever_.IdentifierExtensions) {
	    if (!Actions.contains(action_str)) { return false; }
	    IdentifierExtensions[identifier_ext] = action_str;
	}

	return true;
    }
};
