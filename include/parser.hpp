#pragma once

#include "file_retriever.hpp"
#include "directory_retriever.hpp"
#include "config_directory.hpp"
class MenuController {};

#include <set>
#include <unordered_map>
#include <string>
#include <functional>

class Parser {
    std::set<std::string> Scripts_;
    std::unordered_map<std::string, std::string> IdentifierExtension_To_Action_;

public:
    static struct Delimiter {
	static constexpr char ScriptAction = '|';
	static constexpr char ProgramAction = ':';
	static constexpr char OptionListMenuAction = '~';
	static constexpr char ActionMenuAction = '@';

	static constexpr std::string ActionAll = "~@:|";

	static constexpr char Data = '>';

	static constexpr char IdentifierExtension = '.';
    } Delimiter;

    static struct ProgramAction {
	static constexpr std::string Nothing = "nothing";
    } ProgramAction;

    static const inline std::unordered_map<std::string, std::function<int(std::string, MenuController*)>> ProgramAction_String_To_Function_ {
	{ ProgramAction::Nothing, [](std::string data, MenuController* menu_controller) { return 0; }},
    };

    Parser(const ConfigDirectory& config_directory)
	: Scripts_Retriever(config_directory.GetScriptsDirectoryPath()), IdentifierExtension_To_Action_Retriever(config_directory.GetIdentifierExtensionsFilePath())
    {}

    DirectoryRetriever Scripts_Retriever;
    FileRetriever IdentifierExtension_To_Action_Retriever;

    void LoadScripts() {
	for (auto script: Scripts_Retriever.GetData()) {
	    std::cout << script << std::endl;
	    Scripts_.emplace(script);
	}
    }

    bool LoadIdentifierExtensions() {
	for (auto identifier_extension_to_script: IdentifierExtension_To_Action_Retriever.GetData()) {
	    std::cout << identifier_extension_to_script << std::endl;
	    size_t pos = identifier_extension_to_script.find(Delimiter::Data);

	    std::string name;
	    std::string data;

	    if (pos == std::string::npos) {
		std::cerr << "Error: Malformed option list - missing \'>\'" << std::endl;
		return false;
	    } else {
		name = identifier_extension_to_script.substr(0, pos);
		data = identifier_extension_to_script.substr(pos + 1);
		IdentifierExtension_To_Action_[name] = data;
	    }

	}

	return true;
    }

    int ExecuteOptionString(const std::string& option_string) {
	size_t data_pos = option_string.find_first_of(Delimiter::Data);
	
	std::string name;
	std::string data;
	std::string action;

	bool data_del_found;
	if (data_pos == std::string::npos) {
	    data_del_found = false;
	    name = option_string;
	} else {
	    data_del_found = true;
	    name = option_string.substr(0, data_pos);
	    data = option_string.substr(data_pos + 1);
	}

	size_t action_pos = name.find_first_of(Delimiter::ActionAll);

	if (action_pos == std::string::npos) {
	    std::cout << 1 << std::endl;
	    return ExecuteDataDefault(option_string);
	}

	action = option_string.substr(action_pos);

	if (!data_del_found) {
	    data = option_string.substr(0, action_pos - 1);
	}

	if (!ValidAction(action)) {
	    std::cout << 2 << action << std::endl;
	    data = option_string;
	    action = ProgramAction::Nothing;

	    return ExecuteDataDefault(option_string);
	}

	std::cout << 3 << std::endl;
	return Execute(action, data);
    }

    int ExecuteDataDefault(const std::string& option_string) {
	size_t identifier_extension_pos = option_string.find_first_of(Delimiter::IdentifierExtension);
	std::string identifier_extension;

	if (identifier_extension_pos == std::string::npos) {
	    identifier_extension = "";
	} else {
	    identifier_extension = option_string.substr(identifier_extension_pos + 1);
	}

	std::cout << "id" << identifier_extension << std::endl;

	std::string action;
	if (IdentifierExtension_To_Action_.contains(identifier_extension)) {
	    action = IdentifierExtension_To_Action_[identifier_extension];
	} else {
	    action = Delimiter::ProgramAction + ProgramAction::Nothing;
	}

	return Execute(action, option_string);
    }

    bool ValidAction(const std::string& action) {
	char action_delimiter = action[0];
	std::string action_identifier = action.substr(1);
	std::cout << "del:" << action_delimiter << std::endl;
	std::cout << "id:" << action_identifier << std::endl;

	switch (action_delimiter) {
	    case (Delimiter::ActionMenuAction):
		return ValidAction(action_identifier);
		break;
	    case (Delimiter::OptionListMenuAction):
		return ValidAction(action_identifier);
		break;
	    case (Delimiter::ProgramAction):
		return ProgramAction_String_To_Function_.contains(action_identifier);
		break;
	    case (Delimiter::ScriptAction):
		return Scripts_.contains(action_identifier);
		break;
	}

	return false;
    }

    int Execute(const std::string& action, const std::string& data) {
	char action_delimiter = action[0];
	std::string action_identifier = action.substr(1);

	std::cout << "action: " << action << std::endl;
	std::cout << "data: " << data << std::endl;

	switch (action_delimiter) {
	    case (Delimiter::ActionMenuAction):
		break;
	    case (Delimiter::OptionListMenuAction):
		break;
	    case (Delimiter::ProgramAction):
		break;
	    case (Delimiter::ScriptAction):
		break;
	}

	return -1;
    }

    //void EvaluateData(const std::string& script, const std::string& name);
};
