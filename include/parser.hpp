#pragma once

#include "config_directory.hpp"

#include <set>
#include <unordered_map>
#include <string>
#include <functional>

#include "option_list.hpp"

#define PARSER_EXIT_STAY_IN_MENU 999

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

    struct Delimiter {
	static constexpr wchar_t ScriptAction = L'|';
	static constexpr wchar_t ProgramAction = L'~';
	static constexpr wchar_t DestinationAction = L'@';
	static constexpr wchar_t MenuAction = L':';

	const std::wstring ActionAll = L"~@:|";

	static constexpr wchar_t Data = L'>';

	static constexpr wchar_t IdentifierExtension = L'.';
    } Delimiter;

    static const std::unordered_map<std::wstring, std::function<int(std::wstring)>> ProgramAction_String_To_Function;
    static const std::unordered_map<std::wstring, std::function<std::unique_ptr<OptionList>()>> DestinationAction_String_To_Function;

    ConfigDirectory& Config_Directory = ConfigDirectory::Instance();

    void LoadScripts();
    bool LoadIdentifierExtensions();

    int ExecuteOptionString(const std::wstring& option_string);
    int ExecuteDataDefault(const std::wstring& option_string);
    bool ValidAction(const std::wstring& action);
    int Execute(const std::wstring& action, const std::wstring& data);
};
