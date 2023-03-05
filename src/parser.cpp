#include "parser.hpp"

#include "option_list.hpp"
#include "menu_tui.hpp"

#include "data_option_list.hpp"

#define EXIT_STAY_IN_MENU 999

const std::unordered_map<std::string, std::function<int(std::string)>> Parser::ProgramAction_String_To_Function_ {
    { ProgramAction::Nothing, [](std::string data) { std::cout << "hello :)" << std::endl; return 0; }},
    { ProgramAction::Echo, [](std::string data) { std::cout << data << std::endl; return 0; }},
};

const std::unordered_map<std::string, std::function<std::unique_ptr<OptionList>(std::string)>> Parser::DestinationAction_String_To_Function {
    //{ DestinationAction::Directory, [](std::string data) { ; }},
    //{ DestinationAction::ReadDirectory, [](std::string data) { return 0; }},
    //{ DestinationAction::BookmarkList, [](std::string data) { return 0; }},
    { DestinationAction::DataList, [](std::string data) { return std::make_unique<DataOptionList>(DataOptionList(data)); }},
};
void Parser::LoadScripts() {
    for (auto script: Config_Directory.Scripts_Retriever->GetData()) {
	Scripts_.emplace(script);
    }
}

bool Parser::LoadIdentifierExtensions() {
    for (auto identifier_extension_to_script: Config_Directory.IdentifierExtension_To_Action_Retriever->GetData()) {
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

int Parser::ExecuteOptionString(const std::string& option_string) {
    size_t data_pos = option_string.find_first_of(Delimiter::Data);
    size_t action_pos = option_string.find_first_of(Delimiter::ActionAll);


    std::string data;
    if (action_pos == std::string::npos) {
	return ExecuteDataDefault(option_string);
    } else if (data_pos == std::string::npos) {
	data = option_string.substr(0, action_pos);
	data_pos = option_string.size();
    } else if (action_pos > data_pos) {
	return ExecuteDataDefault(option_string);
    } else {
	data = option_string.substr(data_pos + 1);
    }

    std::string action_string = option_string.substr(action_pos, data_pos - action_pos);

    if (!ValidAction(action_string)) {
	return ExecuteDataDefault(option_string);
    } else {
	return Execute(action_string, data);
    }
}

int Parser::ExecuteDataDefault(const std::string& option_string) {
    size_t identifier_extension_pos = option_string.find_first_of(Delimiter::IdentifierExtension);
    std::string identifier_extension;

    if (identifier_extension_pos == std::string::npos) {
	identifier_extension = "";
    } else {
	identifier_extension = option_string.substr(identifier_extension_pos + 1);
    }

    std::string action;
    if (IdentifierExtension_To_Action_.contains(identifier_extension)) {
	action = IdentifierExtension_To_Action_[identifier_extension];
    } else {
	action = Delimiter::ProgramAction + ProgramAction::Nothing;
    }

    return Execute(action, option_string);
}

bool Parser::ValidAction(const std::string& action) {
    char action_delimiter = action[0];

    std::string action_identifier = action.substr(1);

    size_t next_action_pos = action_identifier.find_first_of(Delimiter::ActionAll);

    switch (action_delimiter) {
	case (Delimiter::DestinationAction):
	    if (next_action_pos == std::string::npos) { return DestinationAction_String_To_Function.contains(action_identifier); }
	    else if (!DestinationAction_String_To_Function.contains(action_identifier.substr(0, next_action_pos))) { return false; }
	    else { return ValidAction(action_identifier.substr(next_action_pos)); }
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

int Parser::Execute(const std::string& action, const std::string& data) {
    char action_delimiter = action[0];
    std::string action_identifier = action.substr(1);

    std::cout << "action: " << action << std::endl;
    std::cout << "data: " << data << std::endl;

    switch (action_delimiter) {
	case (Delimiter::DestinationAction):
	{
	    size_t next_action_pos = action_identifier.find_first_of(Delimiter::ActionAll);

	    std::string destination_action;
	    std::string destination_default = "";

	    if (next_action_pos == std::string::npos) {
		destination_action = action_identifier;
	    } else {
		destination_action = action_identifier.substr(0, next_action_pos);
		destination_default = action_identifier.substr(next_action_pos);
	    }

	    std::unique_ptr<OptionList> option_list = std::move(DestinationAction_String_To_Function.at(action_identifier)(data));
	    option_list->Load();

	    MenuTUI menu_tui = MenuTUI(std::move(option_list), data, action);
	    menu_tui.Start();

	    int ret;
	    while((ret = Execute(destination_default, menu_tui.Input())) == EXIT_STAY_IN_MENU);

	    //menu_tui.Close();

	    return ret;
	    //menu_controller = ...;
	    break;
	}

	case (Delimiter::ProgramAction):
	    ProgramAction_String_To_Function_.at(action_identifier)(data);
	    break;
	case (Delimiter::MenuAction):
	    if (menu_controller_ == nullptr) {
		std::cerr << "Error: you can't execute a menu action here" << std::endl;
	    } else {
		return EXIT_STAY_IN_MENU;
	    }

	    break;
	case (Delimiter::ScriptAction):
	    std::cout <<((Config_Directory.GetScriptsDirectoryPath() / action_identifier).string() + " " + "\"" + data + "\"") << std::endl;
	    return std::system(((Config_Directory.GetScriptsDirectoryPath() / action_identifier).string() + " " + "\"" + data + "\"").c_str());
	    break;
    }

    return -1;
}
