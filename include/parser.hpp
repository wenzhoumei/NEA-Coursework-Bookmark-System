#pragma once

#include "config_directory.hpp"

#include <set>
#include <unordered_map>
#include <string>
#include <functional>
#include "menu_controller.hpp"

#include "option_list.hpp"

class Parser {
    std::set<std::string> Scripts_;
    std::unordered_map<std::string, std::string> IdentifierExtension_To_Action_;

    MenuController* menu_controller_ = nullptr;
public:
    static struct Delimiter {
	static constexpr char ScriptAction = '|';
	static constexpr char ProgramAction = '~';
	static constexpr char DestinationAction = '@';
	static constexpr char MenuAction = ':';

	static constexpr std::string ActionAll = "~@:|";

	static constexpr char Data = '>';

	static constexpr char IdentifierExtension = '.';
    } Delimiter;

    static struct DestinationAction {
	static constexpr std::string Directory = "dir";
	static constexpr std::string ReadDirectory = "rdir";
	static constexpr std::string BookmarkList = "bmk";
	static constexpr std::string DataList = "file";
    } DestinationAction;

    static struct MenuAction {
	static constexpr std::string Directory = "add";
	static constexpr std::string ReadDirectory = "remove";
    } MenuAction;

    static struct ProgramAction {
	static constexpr std::string Nothing = "nothing";
	static constexpr std::string Echo = "echo";
    } ProgramAction;

    static const std::unordered_map<std::string, std::function<int(std::string)>> ProgramAction_String_To_Function_;

    static const std::unordered_map<std::string, std::function<std::unique_ptr<OptionList>(std::string)>> DestinationAction_String_To_Function;

    ConfigDirectory Config_Directory;

    void LoadScripts();
    bool LoadIdentifierExtensions();

    int ExecuteOptionString(const std::string& option_string);
    int ExecuteDataDefault(const std::string& option_string);
    bool ValidAction(const std::string& action);
    int Execute(const std::string& action, const std::string& data);
};
