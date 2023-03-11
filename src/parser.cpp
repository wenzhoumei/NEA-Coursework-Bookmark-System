#include "parser.hpp"

#include "option_list/option_list.hpp"
#include "menu_tui.hpp"
#include "exit_code.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
#include <ncurses.h>

#include "menu_controller/menu_controller.hpp"

const std::unordered_map<std::wstring, std::function<void(MenuController*)>> Parser::MenuAction_String_To_Function {
    { L"todo", [](MenuController* menu_controller) { menu_controller->ProcessChar(CTRL_MASK('r')); }},
    { L"flashcard", [](MenuController* menu_controller) { menu_controller->ProcessChar(CTRL_MASK('d')); }},
};

bool Parser::LoadScripts() {
    for (auto script: Config_Directory.Scripts_Retriever->GetData()) {
	Scripts_.emplace(script);
    }

    return true;
}

bool Parser::LoadIdentifierExtensions() {
    for (auto identifier_extension_to_script: Config_Directory.IdentifierExtension_To_Action_Retriever->GetData()) {
	size_t pos = identifier_extension_to_script.find(Data.Delimiter);

	std::wstring name;
	std::wstring data;

	if (pos == std::wstring::npos) {
	    my::log.Error(1) << "Malformed option list - missing \'>\'";
	    return false;
	} else {
	    name = identifier_extension_to_script.substr(0, pos);
	    data = identifier_extension_to_script.substr(pos + 1);

	    if (GetActionPos_(data) != 0) {
		my::log.Warning() << "Ignoring invalid action assigned to file extension: " << data;
	    } else {
		IdentifierExtension_To_Action_[name] = data;
	    }
	}

    }

    return true;
}

int Parser::ExecuteOptionString(const std::wstring& option_string) {
    std::wstring data;
    size_t data_pos = option_string.find_first_of(Data::Delimiter);

    std::wstring name;
    if (data_pos == std::wstring::npos) {
	name = option_string;
    } else {
	name = option_string.substr(0, data_pos);
    }

    size_t action_pos = GetActionPos_(name);

    if (action_pos == std::wstring::npos) {
	// file.txt
	return ExecuteDataDefault(option_string);
    }

    if (data_pos == std::wstring::npos) {
	// file.txt|text
	data = option_string.substr(0, action_pos);
    } else {
	// file.txt|text>/path/to/file.txt
	data = option_string.substr(data_pos + 1);
    }

    std::wstring action_string = name.substr(action_pos);
    return Execute(action_string, data);
}

int Parser::ExecuteDataDefault(const std::wstring& option_string) {
    size_t identifier_extension_pos = option_string.find_first_of(IdentifierExtension::Delimiter);
    std::wstring identifier_extension;

    if (identifier_extension_pos == std::wstring::npos) {
	identifier_extension = L"";
    } else {
	identifier_extension = option_string.substr(identifier_extension_pos + 1);
    }

    std::wstring action;
    if (IdentifierExtension_To_Action_.contains(identifier_extension)) {
	action = IdentifierExtension_To_Action_[identifier_extension];
    } else {
	action = std::wstring(1, ProgramAction::Delimiter) + ProgramAction.Nothing;
    }

    return Execute(action, option_string);
}

void Parser::SetMenuController(MenuController* menu_controller) {
    menu_controller_ = menu_controller;
}

int Parser::Execute(const std::wstring& action, const std::wstring& data) {
    wchar_t action_delimiter = action[0];
    std::wstring action_identifier = action.substr(1);

    my::log.Info() << L"action (" << action << "), " << "data (" << data << ")" << std::endl;

    my::log.Debug() << L"actiondelimiter (" << action_delimiter << ")" << std::endl;

    switch (action_delimiter) {
	case (DestinationAction::Delimiter):
	{
	    std::wstring action_at_destination = std::wstring(1, ProgramAction::Delimiter) + ProgramAction.OptionString;

	    size_t i = 0;
	    std::wstring destination_action_identifier;
	    for (wchar_t c: action_identifier) {
		i++;
		if (IsActionDelimiter_(c)) {
		    action_at_destination = action_identifier.substr(i - 1);
		    break;
		}

		destination_action_identifier += c;
	    }

	    if (action_at_destination == L"") {
		action_at_destination = std::wstring(1, ProgramAction::Delimiter) + ProgramAction.OptionString;
	    }

	    MenuTUI menu_tui = MenuTUI(DestinationAction_String_To_Function.at(destination_action_identifier)(action_at_destination, action, data));

	    return menu_tui.Open();

	    break;
	}
	case (ProgramAction::Delimiter):
	    return ProgramAction_String_To_Function.at(action_identifier)(data);
	    break;
	case (MenuAction::Delimiter):
	    if (menu_controller_ == nullptr) {
		my::log.Error(1) << "You can't execute a menu action here";
	    } else {
		my::log.Debug() << " theory4" << std::endl;
		MenuAction_String_To_Function.at(action_identifier)(menu_controller_);
		return ExitCode::DontExit;
	    }

	    break;
	case (ScriptAction::Delimiter):
	{
	    // convert wstring data to string
	    std::string data_str(data.begin(), data.end());

	    std::cout <<((Config_Directory.GetScriptsDirectoryPath() / action_identifier).string() + " " + "\"" + data_str + "\"") << std::endl;
	    return std::system(((Config_Directory.GetScriptsDirectoryPath() / action_identifier).string() + " " + "\"" + data_str + "\"").c_str());
	    break;
	}
    }

    return -1;
}
