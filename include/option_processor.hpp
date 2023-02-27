#pragma once

#include "option.hpp"
#include "action_processor.hpp"

#include "settings.hpp"

class OptionProcessor {
private:
    OptionProcessor(): settings_(Settings::Instance()), action_processor_(ActionProcessor::Instance()) {}; // Private constructor to prevent instantiation
    OptionProcessor(const OptionProcessor&) = delete; // Delete copy constructor
    OptionProcessor& operator=(const OptionProcessor&) = delete; // Delete assignment operato

    Settings& settings_;
    ActionProcessor& action_processor_;

    std::unordered_map<std::string, std::function<int(Option)>> ProgramActions {
    };

public:
    const struct Delimiter {
	const char Data = ',';

	const char IdentifierExtension = '.';
    } Delimiter;

    static OptionProcessor& Instance()
    {
	static OptionProcessor INSTANCE;
	return INSTANCE;
    }

    Option CreateOption(const std::string& option_str) {
	size_t data_delimiter_pos = option_str.find_last_of(Delimiter.Data);
	size_t identifier_extension_delimiter_pos = option_str.find_first_of(Delimiter.IdentifierExtension);

	char data_delimiter = option_str[data_delimiter_pos];

	if (identifier_extension_delimiter_pos > data_delimiter_pos) {
	    Action default_action = action_processor_.GetDefaultAction();
	    return CreateOption(option_str, default_action);
	}

	std::string script_maybe = option_str.substr(identifier_extension_delimiter_pos, data_delimiter_pos);
    }

    Option CreateOption(const std::string& option_str, const Action& action) {
	return Option { option_str, option_str, action };
    }

    int ExecuteOption() {
	/*
	std::string command = script_path_.string() + " " + data_;
	return WEXITSTATUS(std::system(command.c_str()));
	*/
    }

private:
    bool IsValidScript_(const std::string& script_maybe) {
    }
};
