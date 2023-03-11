#pragma once

#include "config_directory.hpp"

#include <set>
#include <unordered_map>
#include <string>
#include <functional>

#include "option_list/option_list.hpp"
#include "option_list/data_option_list.hpp"
#include "option_list/bookmark_option_list.hpp"
#include "option_list/read_directory_option_list.hpp"
#include "option_list/editable_directory_option_list.hpp"
#include "option_list/read_only_data_option_list.hpp"

class MenuController;
class Parser {
    std::set<std::wstring> Scripts_;
    std::unordered_map<std::wstring, std::wstring> IdentifierExtension_To_Action_;

    MenuController* menu_controller_ = nullptr;

    Parser() {}; // Private constructor to prevent instantiation
    Parser(const Parser&) = delete; // Delete copy constructor
    Parser& operator=(const Parser&) = delete; // Delete assignment operato

public:
    static Parser& Instance()
    {
	static Parser INSTANCE;
	return INSTANCE;
    }

    struct Data {
	static constexpr wchar_t Delimiter = L'>';
    } Data;

    struct IdentifierExtension {
	static constexpr wchar_t Delimiter = L'.';
    } IdentifierExtension;

    struct ProgramAction {
	static constexpr wchar_t Delimiter = L'~';
	const std::wstring Nothing = L"nothing";
	const std::wstring Echo = L"echo";
	const std::wstring OptionString = L"option_string";
    } ProgramAction;

    struct DestinationAction {
	DestinationAction() {}
	static constexpr const wchar_t Delimiter = L'@';
    } DestinationAction;

    struct ScriptAction {
	ScriptAction() {}
	static constexpr wchar_t Delimiter = L'|';
    } ScriptAction;

    struct MenuAction {
	MenuAction() {}
	static constexpr wchar_t Delimiter = L':';
    } MenuAction;

    const std::unordered_map<std::wstring, std::function<int(std::wstring)>> ProgramAction_String_To_Function {
	{ ProgramAction.Nothing, [](std::wstring data) { std::cout << "hello :)" << std::endl; return 0; }},
	{ ProgramAction.Echo, [](std::wstring data) { std::wcout << data << std::endl; return 0; }},
	{ ProgramAction.OptionString, [this](std::wstring data) { return ExecuteOptionString(data); }},
    };

    const std::unordered_map<std::wstring, std::function<std::unique_ptr<OptionList>(std::wstring, std::wstring, std::wstring)>> DestinationAction_String_To_Function {
	{ L"rdir", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return std::make_unique<ReadDirectoryOptionList>(ReadDirectoryOptionList(action_out_of_here, action_to_here, location)); }},
	{ L"dir", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return std::make_unique<DirectoryOptionList>(DirectoryOptionList(action_out_of_here, action_to_here, location)); }},
	{ L"bmk", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return std::make_unique<BmkOptionList>(BmkOptionList(action_out_of_here, action_to_here, location)); }},
	{ L"file", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return std::make_unique<DataOptionList>(DataOptionList(action_out_of_here, action_to_here, location)); }},
	{ L"rfile", [](std::wstring action_out_of_here, std::wstring action_to_here, std::wstring location) { return std::make_unique<ReadOnlyDataOptionList>(ReadOnlyDataOptionList(action_out_of_here, action_to_here, location)); }},
    };

    const std::unordered_map<std::wstring, std::function<std::unique_ptr<OptionList>(std::wstring)>> MenuAction_String_To_Function {
    };

    ConfigDirectory& Config_Directory = ConfigDirectory::Instance();

    bool LoadScripts();
    bool LoadIdentifierExtensions();

    void SetMenuController(MenuController* menu_controller_);

    int ExecuteOptionString(const std::wstring& option_string);
    int ExecuteDataDefault(const std::wstring& option_string);
    int Execute(const std::wstring& action, const std::wstring& data);

private:
    size_t GetActionPos_(const std::wstring& name, size_t action_pos=std::wstring::npos, bool first_it=true) const {
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

    size_t FindLastActionDelimiterPos_(const std::wstring& str) const {
	size_t pos = str.length();
	while (pos > 0) {
	    --pos;
	    if (IsActionDelimiter_(str[pos])) {
		return pos;
	    }
	}
	
	return std::wstring::npos;
    }

    size_t FindFirstActionDelimiterPos_(const std::wstring& str) const {
	size_t pos = 0;
	while (pos < str.length()) {
	    if (IsActionDelimiter_(str[pos])) {
		return pos;
	    }
	    ++pos;
	}
	return std::wstring::npos;
    }

    bool IsActionDelimiter_(const wchar_t& c) const {
	return c == ProgramAction.Delimiter
	    || c == DestinationAction.Delimiter
	    || c == ScriptAction.Delimiter
	    || c == MenuAction.Delimiter;
    }

    bool IsValidAction_(const wchar_t& action_del, const std::wstring& action_identifier) const {
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

	Log::Instance().Error(9) << "Invalid action delimiter: " << action_del;
	return false;
    }
};
