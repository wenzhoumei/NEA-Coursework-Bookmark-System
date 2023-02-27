#pragma once

#include "action.hpp"
#include "menu_controller.hpp"
#include <unordered_map>

class ActionProcessor {
private:
    ActionProcessor(): settings_(Settings::Instance()) {}; // Private constructor to prevent instantiation
    ActionProcessor(const ActionProcessor&) = delete; // Delete copy constructor
    ActionProcessor& operator=(const ActionProcessor&) = delete; // Delete assignment operato

    Settings& settings_;
public:

    static struct Delimiter {
	static constexpr char Menu = '@';
	static const char Script = '|';
	static const char Program = ':';

	// TODO
	static const char Output = '~';
    } Delimiter;

    static ActionProcessor& Instance()
    {
	static ActionProcessor INSTANCE;
	return INSTANCE;
    }

    std::unordered_map<std::string, std::function<int(std::string, MenuController*)>> ProgramActions {
	{ "nothing", [](std::string data, MenuController* menu_controller) { return 0; }},
    };

    Action GetDefaultAction() {
	if (settings_.IdentifierExtensions.contains(settings_.IdentifierExtension.None)) {
	    return CreateAction(settings_.IdentifierExtensions[settings_.IdentifierExtension.None]);
	} else {
	    return CreateAction(Delimiter.Program, "nothing");
	}
    }

    bool IsValid(const std::string& action_str)
    {
	char action_delimiter = action_str[0];
	
	return IsValid(action_delimiter, action_str.substr(1));
    }

    bool IsValid(const char& action_delimiter, const std::string& action_identifier)
    {
	switch (action_delimiter) {
	    case (Delimiter.Menu):
	    case (Delimiter.Script):
		if (!settings_.Scripts.contains(action_identifier)) return false;
		else return true;
		break;
	    case (Delimiter.Program):
		if (!ProgramActions.contains(action_identifier)) return false;
		else return true;
		break;
	    default:
		return false;
		break;
	}
    }

    Action CreateAction(const std::string& action_str) {
	char action_delimiter = action_str[0];
	return CreateAction(action_delimiter, action_str.substr(1));
    }

    Action CreateAction(const char& action_delimiter, const std::string& action_identifier) {
	return Action { action_delimiter, action_identifier };
    }
};
