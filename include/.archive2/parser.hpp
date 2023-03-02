#pragma once

#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <functional>
#include <set>
#include <boost/algorithm/string.hpp>

#include "config_writer.hpp"
#include "config_retriever.hpp"
#include "menu_controller.hpp"

class ParserData {
private:
    ParserData() {}; // Private constructor to prevent instantiation
    ParserData(const ParserData&) = delete; // Delete copy constructor
    ParserData& operator=(const ParserData&) = delete; // Delete assignment operato

public:
    static ParserData& Instance()
    {
	static ParserData INSTANCE;
	return INSTANCE;
    }

    std::unordered_map<std::string, std::function<int(std::string, MenuController*)>> ProgramActions {
	{ "nothing", [](std::string data, MenuController* menu_controller) { return 0; }},
    };

    static constexpr char DelimiterActionMenu = '@';
    static constexpr char DelimiterActionScript = '|';
    static constexpr char DelimiterActionProgram = ':';

    static constexpr char DelimiterData = ',';
    static constexpr char DelimiterIdentifierExtension = '.';

    std::set<std::string> Scripts = {};
    std::set<std::string> OptionLists = {};

    std::unordered_map<std::string, std::string> IdentifierExtensions = {};
};

class ParserDataLoadingCheck {
public:
    bool IdentifierExtension(std::string extension)
    {
	// Check if extension is empty
	if (extension.empty()) {
	    return false;
	}

	// Check if extension contains only valid characters
	for (char c : extension) {
	    if (!std::isalnum(c) && c != '_' && c != '-') {
		return false;
	    }
	}

	// All checks passed, extension is valid
	return true;
    }

    bool OptionList(std::string script_name) {
	return IsOnlyLowerOrUnderscore_(script_name) && !script_name.empty();
    }

    bool Script(std::string script_name) {
	return IsOnlyLowerOrUnderscore_(script_name) && !script_name.empty();
    }

private: 
    ParserData& parser_data_ = ParserData::Instance();

    bool IsOnlyLowerOrUnderscore_(const std::string& str) {
	for (char c : str) {
	    if (!islower(c) || c == '_') {
		return false;
	    }
	}
	return true;
    }
};

class ParserDataLoadedCheck {
public:
    bool Action(const std::string& action_name) {
	char action_delimiter = action_name[0];
	std::string action_identifier = action_name.substr(1);

	switch (action_delimiter) {
	    case(ParserData::DelimiterActionScript):
		return parser_data_.Scripts.contains(action_identifier);
		break;
	    case(ParserData::DelimiterActionProgram):
		return parser_data_.ProgramActions.contains(action_identifier);
		break;
	    default:
		return false;
		break;
	}
    }
private: 
    ParserData& parser_data_ = ParserData::Instance();
};

class ParserDataControl
{
public: 
    ConfigRetriever ConfigDirectoryRetriever;
    ConfigWriter ConfigDirectoryWriter;
    ParserDataLoadingCheck LoadingCheck;
    ParserDataLoadedCheck LoadedCheck;

    bool AddValidScripts()
    {
	for (const auto& script_name: ConfigDirectoryRetriever.Scripts) {
	    if (!LoadingCheck.Script(script_name)) { return false; }
	}

	return true;
    }

    bool AddValidIdentifierExtensions()
    {
	for (const auto& [identifier_extension_name, action_name]: ConfigDirectoryRetriever.IdentifierExtensions) {
	    if (!LoadingCheck.IdentifierExtension(identifier_extension_name)) { return false; }
	    if (!LoadedCheck.Action(identifier_extension_name)) { return false; }
	}

	return true;
    }

    bool AddValidOptionLists()
    {
	for (const auto& option_list_name: ConfigDirectoryRetriever.Scripts) {
	    if (!LoadingCheck.OptionList(option_list_name)) { return false; }
	}

	return true;
    }

private:
    ParserData& parser_data_ = ParserData::Instance();
};
