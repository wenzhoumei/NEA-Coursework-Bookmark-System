#pragma once

#include <set>
#include <unordered_map>
#include <string>
#include <functional>
#include "config_directory.hpp"
#include "log.hpp"
#include "exit_code.hpp"

#include "option_list/option_list.hpp"

class MenuController;

/**
 * @brief Processes and exits texts taking in different information in different contexts, only one loaded during course of program and information used by multiple programs so implemented as a singleton
 */
class Parser {
    std::set<std::wstring> Scripts_;
    std::unordered_map<std::wstring, std::wstring> IdentifierExtension_To_Action_;

    MenuController* menu_controller_ = nullptr;

    Parser() {}; // Private constructor to prevent instantiation
    Parser(const Parser&) = delete; // Delete copy constructor
    Parser& operator=(const Parser&) = delete; // Delete assignment operato

public:
    /**
     * @brief Access point to only instance of class
     *
     * @return Meyer's singleton
     */
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
	{ ProgramAction.Echo, [](std::wstring data) { my::log.Write(L"Echoing: " + data); return 0; }},
	{ ProgramAction.OptionString, [this](std::wstring data) { return ExecuteOptionString(data); }},
	{ ProgramAction.Nothing, [](std::wstring data) { my::log.Write(L"Info: Nothing executed"); return ExitCode::DontExit; }},
    };

    static const std::unordered_map<std::wstring, std::function<OptionList*(std::wstring, std::wstring, std::wstring)>> DestinationAction_String_To_Function;
    static const std::unordered_map<std::wstring, std::function<void(MenuController*)>> MenuAction_String_To_Function;

    ConfigDirectory& Config_Directory = ConfigDirectory::Instance();

    /**
     * @brief Gets data from Config_Directory buffer, acts as another layer to ensure only valid data is loaded
     *
     * @return If it was successful
     */
    bool LoadScripts();

    /**
     * @brief Gets data from Config_Directory buffer, acts as another layer to ensure only valid data is loaded
     *
     * @return If it was successful
     */
    bool LoadIdentifierExtensions();

    /**
     * @brief Sets pointer to current MenuController in use
     * 
     * Set menu_controller_ to nullptr if no menu controller in use
     */
    void SetMenuController(MenuController* menu_controller_);

    /**
     * @brief Evaluates option string and executes it accordingly
     *
     * @return Exit code of program
     */
    int ExecuteOptionString(const std::wstring& option_string);

    /**
     * @brief Evaluates option string as data, option strings that are not defined with an action are funneled into here
     *
     * @return Exit code of program
     */
    int ExecuteDataDefault(const std::wstring& option_string);

    /**
     * @brief Final exit point of all Execute- functions. Execution of code is done here directly
     *
     * @return Exit code of program
     */
    int Execute(const std::wstring& action, const std::wstring& data);
private:
    /**
     * @brief Replaces all instances a certain string to a another string
     *
     * @param str String to change
     * @param to String to find
     * @param from String to replace with
     */
    void ReplaceAll_(std::wstring& str, const std::wstring& from, const std::wstring& to);

    /**
     * @brief Finds position of action by recursively peeling back last action
     *
     * @param name Name of option string - part that will be displayed in a bookmark menu
     * @param action_pos Current action pos, used for logic in recursive function, should not be changed when calling function
     * @param first_it If it is the first iteration of recursion, used for internal logic
     *
     * @return Index of valid action delimiter of the action found or if there was no valid action found return std::wstring::npos
     */
    size_t GetActionPos_(const std::wstring& name, size_t action_pos=std::wstring::npos, bool first_it=true) const;

    /**
     * @brief Finds position of last occurence any action delimiter
     *
     * @param str String to look for it in
     *
     * @return Index of last occurence of action delimiter or std::wstring::npos if not found
     */
    size_t FindLastActionDelimiterPos_(const std::wstring& str) const;

    /**
     * @brief Finds position of first occurence any action delimiter
     *
     * @param str String to look for it in
     * 
     * @return Index of first occurence of action delimiter or std::wstring::npos if not found
     */
    size_t FindFirstActionDelimiterPos_(const std::wstring& str) const;

    /**
     * @brief Checks if a character is a action delimiter
     *
     * @param c Character to check
     *
     * @return If a character is a action delimiter
     */
    bool IsActionDelimiter_(const wchar_t& c) const;

    /**
     * @brief Checks if the action identifier is valid given the action delimiter, which should always be an existing action delimiter according to program logic, throws logic error if it is not
     *
     * @param action_del Action delimiter being used
     * @param action_identifier Action identifier being used
     *
     * @return If the action is valid
     */
    bool IsValidAction_(const wchar_t& action_del, const std::wstring& action_identifier) const;
};
