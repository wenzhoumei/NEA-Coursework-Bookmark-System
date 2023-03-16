#include "parser.hpp"

#include "option_list/option_list.hpp"
#include "menu_tui.hpp"
#include "log.hpp"

#define CTRL_MASK(c) ((c) & 0x1f)
#define KEY_ESCAPE 27
#include <ncurses.h>

#include "option_list/data_option_list.hpp"
#include "option_list/bookmark_option_list.hpp"
#include "option_list/read_directory_option_list.hpp"
#include "option_list/editable_directory_option_list.hpp"
#include "option_list/read_only_data_option_list.hpp"
#include "option_list/scripts_directory_option_list.hpp"
#include "option_list/input_only_option_list.hpp"
#include "menu_controller/menu_controller.hpp"

const std::unordered_map<std::wstring, std::function<void(MenuController*)>> Parser::MenuAction_String_To_Function = {
    { L"todo", [](MenuController* menu_controller) {
		if (menu_controller->WasInput()) { menu_controller->ProcessChar(CTRL_MASK('a')); }
		else { menu_controller->ProcessChar(CTRL_MASK('r')); }
    }},
    { L"tmp", [](MenuController* menu_controller) {
		if (menu_controller->WasInput()) { menu_controller->ProcessChar(CTRL_MASK('a')); }
		else { menu_controller->ProcessChar(CTRL_MASK('r')); }
    }},
    { L"scratchpad", [](MenuController* menu_controller) {
		if (menu_controller->WasInput()) { menu_controller->ProcessChar(CTRL_MASK('a')); }
		else {
		    menu_controller->ProcessChar(CTRL_MASK('d'));
		    menu_controller->ProcessChar(CTRL_MASK('e'));
		}
    }},
    { L"flashcard", [](MenuController* menu_controller) {
		    if (menu_controller->WasInput()) { menu_controller->ProcessChar(CTRL_MASK('a')); }
		    else { menu_controller->ProcessChar(CTRL_MASK('d')); }
    }},
    { L"setting", [](MenuController* menu_controller) {
		    if (menu_controller->WasInput()) { menu_controller->ProcessChar(CTRL_MASK('a')); }
		    else { menu_controller->ProcessChar(CTRL_MASK('d')); }
    }},
    { L"toggle", [](MenuController* menu_controller) {
		    if (menu_controller->WasInput()) { menu_controller->ProcessChar(CTRL_MASK('a')); }
		    else { menu_controller->ProcessChar(CTRL_MASK('d')); }
    }},
};

const std::unordered_map<std::wstring, std::function<OptionList*(std::wstring, std::wstring, std::wstring)>> Parser::DestinationAction_String_To_Function = {
    { L"rdir", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return new ReadDirectoryOptionList(action_out_of_here, action_to_here, location); }},
    { L"dir", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return new EditableDirectoryOptionList(action_out_of_here, action_to_here, location); }},
    { L"sdir", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return new ScriptsDirectoryOptionList(action_out_of_here, action_to_here, location); }},
    { L"bmk", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return new BmkOptionList(action_out_of_here, action_to_here, location); }},
    { L"lst", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return new DataOptionList(action_out_of_here, action_to_here, location); }},
    { L"rlst", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return new ReadOnlyDataOptionList(action_out_of_here, action_to_here, location); }},
    { L"", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return new InputOnlyOptionList(action_out_of_here, action_to_here, location); }},
};

bool Parser::LoadScripts() {
    Scripts_.clear();
    for (auto script: Config_Directory.Scripts_Retriever->GetData()) {
	Scripts_.insert(script);
    }

    return true;
}

bool Parser::LoadIdentifierExtensions() {
    IdentifierExtension_To_Action_.clear();

    for (auto identifier_extension_to_script: Config_Directory.IdentifierExtension_To_Action_Retriever->GetData()) {
	size_t pos = identifier_extension_to_script.find(Data.Delimiter);

	std::wstring name;
	std::wstring data;

	if (pos == std::wstring::npos) {
	    my::log.Error(ExitCode::ConfigLoadError) << "Malformed option list - missing \'>\'" << std::endl;
	    return false;
	} else {
	    name = identifier_extension_to_script.substr(0, pos);
	    data = identifier_extension_to_script.substr(pos + 1);

	    if (GetActionPos_(data) != 0) {
		my::log.Warning() << "Ignoring invalid action assigned to file extension: " << data << std::endl;
	    } else {
		IdentifierExtension_To_Action_[name] = data;
	    }
	}

    }

    return true;
}

int Parser::ExecuteOptionString(const std::wstring& option_string) {
    my::log.History(option_string);
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

    if (std::filesystem::is_directory(option_string)) {
	return Execute(std::wstring(1, DestinationAction.Delimiter) + L"dir", option_string);
    }

    if (identifier_extension_pos == std::wstring::npos) {
	identifier_extension = L"";
    } else {
	identifier_extension = option_string.substr(identifier_extension_pos + 1);
    }

    std::wstring action;
    if (IdentifierExtension_To_Action_.contains(identifier_extension)) {
	action = IdentifierExtension_To_Action_[identifier_extension];
    } else {
	if (IdentifierExtension_To_Action_.contains(L"")) {
	    action = IdentifierExtension_To_Action_[L""];
	} else {
	    action = std::wstring(1, ProgramAction::Delimiter) + ProgramAction.Nothing;
	}
    }

    return Execute(action, option_string);
}

void Parser::SetMenuController(MenuController* menu_controller) {
    menu_controller_ = menu_controller;
}

void Parser::ReplaceAll_(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::wstring::npos) {
	str.replace(pos, from.length(), to);
	pos += to.length();
    }
}

int Parser::Execute(const std::wstring& action, const std::wstring& data) {
    wchar_t action_delimiter = action[0];
    std::wstring action_identifier = action.substr(1);

    const std::wstring config_dir_macro = L"CONFIG_DIR";
    std::wstring processed_data = data;
    ReplaceAll_(processed_data, config_dir_macro, Config_Directory.GetPath().wstring());

    my::log.Info() << L"Executing action(" << action << "), data(" << data << ")" << std::endl;

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

	    MenuTUI menu_tui = MenuTUI(DestinationAction_String_To_Function.at(destination_action_identifier)(action_at_destination, action, processed_data));

	    return menu_tui.Open();

	    break;
	}

	case (ProgramAction::Delimiter):
	    return ProgramAction_String_To_Function.at(action_identifier)(processed_data);
	    break;
	case (MenuAction::Delimiter):
	    if (menu_controller_ == nullptr) {
		my::log.Error(ExitCode::UserError) << "You can't execute a menu action here" << std::endl;
	    } else {
		MenuAction_String_To_Function.at(action_identifier)(menu_controller_);
		return ExitCode::DontExit;
	    }

	    break;
	case (ScriptAction::Delimiter):
	{
	    // convert wstring data to string
	    std::string data_str(processed_data.begin(), processed_data.end());

	    my::log.Info() << "Running command: \"" << ("nohup " + (Config_Directory.GetScriptsDirectoryPath() / action_identifier).string() + " " + "\"" + data_str + "\" >/dev/null 2>&1" + "\"").c_str() << std::endl;
	    std::system(("nohup " + (Config_Directory.GetScriptsDirectoryPath() / action_identifier).string() + " " + "\"" + data_str + "\" >/dev/null 2>&1").c_str());
	    return ExitCode::Success;
	}
    }

    return -1;
}

    size_t Parser::GetActionPos_(const std::wstring& name, size_t action_pos, bool first_it) const {
	size_t action_delimiter_pos = FindLastActionDelimiterPos_(name);

	if (action_delimiter_pos == std::wstring::npos) {
	    return action_pos;
	} else {
	    wchar_t action_delimiter = name[action_delimiter_pos];
	    std::wstring action_identifier = name.substr(action_delimiter_pos + 1);

	    if (IsValidAction_(action_delimiter, action_identifier)) {
		std::wstring identifier = name.substr(0, action_delimiter_pos);
		if (action_delimiter != DestinationAction.Delimiter && !first_it) { return action_pos; }
		else { return GetActionPos_(identifier, action_delimiter_pos, false); }
	    } else {
		return action_pos;
	    }
	}
    }

    size_t Parser::FindLastActionDelimiterPos_(const std::wstring& str) const {
	size_t pos = str.length();
	while (pos > 0) {
	    --pos;
	    if (IsActionDelimiter_(str[pos])) {
		return pos;
	    }
	}
	
	return std::wstring::npos;
    }

    size_t Parser::FindFirstActionDelimiterPos_(const std::wstring& str) const {
	size_t pos = 0;
	while (pos < str.length()) {
	    if (IsActionDelimiter_(str[pos])) {
		return pos;
	    }
	    ++pos;
	}
	return std::wstring::npos;
    }

    bool Parser::IsActionDelimiter_(const wchar_t& c) const {
	return c == ProgramAction.Delimiter
	    || c == DestinationAction.Delimiter
	    || c == ScriptAction.Delimiter
	    || c == MenuAction.Delimiter;
    }

    bool Parser::IsValidAction_(const wchar_t& action_del, const std::wstring& action_identifier) const {
	switch (action_del) {
	    case (DestinationAction::Delimiter):
		return DestinationAction_String_To_Function.contains(action_identifier);
		break;
	    case (ProgramAction::Delimiter):
		return ProgramAction_String_To_Function.contains(action_identifier);
		break;
	    case (ScriptAction::Delimiter):
		return Scripts_.contains(action_identifier);
		break;
	    case (MenuAction::Delimiter):
		return MenuAction_String_To_Function.contains(action_identifier);
		break;
	}

	my::log.Error(ExitCode::LogicError) << "Invalid action delimiter: " << action_del;
	return false;
    }
