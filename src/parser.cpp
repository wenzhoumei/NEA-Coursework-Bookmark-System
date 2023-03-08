#include "parser.hpp"

#include "option_list.hpp"
#include "menu_tui.hpp"
#include "exit_code.hpp"

/*
    static struct DestinationAction {
	static constexpr std::wstring Directory = L"dir";
	static constexpr std::wstring ReadDirectory = L"rdr";
	static constexpr std::wstring BookmarkList = L"bmk";
	static constexpr std::wstring DataList = L"lst";
    } DestinationAction;

    static struct MenuAction {
	static constexpr std::wstring Directory = L"add";
	static constexpr std::wstring ReadDirectory = L"rm";
    } MenuAction;

    static struct ProgramAction {
	static constexpr std::wstring Nothing = L"nul";
	static constexpr std::wstring Echo = L"echo";
    } ProgramAction;
*/

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
	    Log::Instance().Error(1) << "Malformed option list - missing \'>\'";
	    return false;
	} else {
	    name = identifier_extension_to_script.substr(0, pos);
	    data = identifier_extension_to_script.substr(pos + 1);

	    if (GetActionPos_(data) != 0) {
		Log::Instance().Warning() << "Ignoring invalid action assigned to file extension: " << data;
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

int Parser::Execute(const std::wstring& action, const std::wstring& data) {
    char action_delimiter = action[0];
    std::wstring action_identifier = action.substr(1);

    Log::Instance().Info() << L"action: " << action;
    Log::Instance().Info() << L"data: " << data;

    switch (action_delimiter) {
	case (DestinationAction::Delimiter):
	{
	    std::wstring action_at_destination = std::wstring(1, ProgramAction::Delimiter) + ProgramAction.OptionString;

	    size_t i = 0;
	    std::wstring destination_action_identifier;
	    for (wchar_t c: action_identifier) {
		i++;
		if (IsActionDelimiter_(c)) {
		    action_at_destination = action_identifier.substr(i);
		    break;
		}

		destination_action_identifier += c;
	    }

	    std::unique_ptr<OptionList> option_list = std::move(DestinationAction_String_To_Function.at(destination_action_identifier)());
	    option_list->Load(data);

	    if (!option_list->SuccessfullyLoaded()) {
		Log::Instance().Warning() << "Failed to load";
		break;
	    }

	    MenuTUI menu_tui = MenuTUI(std::move(option_list), action_at_destination, data);
	    menu_controller_ = menu_tui.GetController();

	    return menu_tui.Open();

	    break;
	}
	case (ProgramAction::Delimiter):
	    ProgramAction_String_To_Function.at(action_identifier)(data);
	    break;
	case (MenuAction::Delimiter):
	    if (menu_controller_ == nullptr) {
		Log::Instance().Error(1) << "You can't execute a menu action here";
	    } else {
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
