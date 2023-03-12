#pragma once


#include <set>
#include <unordered_map>
#include <string>
#include <functional>
#include "config_directory.hpp"

#include "option_list/option_list.hpp"

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

    static const std::unordered_map<std::wstring, std::function<OptionList*(std::wstring, std::wstring, std::wstring)>> DestinationAction_String_To_Function;
    static const std::unordered_map<std::wstring, std::function<void(MenuController*)>> MenuAction_String_To_Function;

    ConfigDirectory& Config_Directory = ConfigDirectory::Instance();
    bool LoadScripts();
    bool LoadIdentifierExtensions();

    void SetMenuController(MenuController* menu_controller_);

    int ExecuteOptionString(const std::wstring& option_string);
    int ExecuteDataDefault(const std::wstring& option_string);

    int ExecuteBookmark(const std::wstring& name, const std::wstring& data);

    int Execute(const std::wstring& action, const std::wstring& data);
private:
    void ReplaceAll_(std::wstring& str, const std::wstring& from, const std::wstring& to);
    size_t GetActionPos_(const std::wstring& name, size_t action_pos=std::wstring::npos, bool first_it=true) const;
    size_t FindLastActionDelimiterPos_(const std::wstring& str) const;
    size_t FindFirstActionDelimiterPos_(const std::wstring& str) const;
    bool IsActionDelimiter_(const wchar_t& c) const;
    bool IsValidAction_(const wchar_t& action_del, const std::wstring& action_identifier) const;
};
