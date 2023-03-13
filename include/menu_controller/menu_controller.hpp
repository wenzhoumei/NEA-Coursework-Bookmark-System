#pragma once
#include "menu_data.hpp"

/**
 * @brief Interface that changes MenuData class
 */
class MenuController {
protected:
    struct PossibleExit { bool Matched; int ReturnCode; };
    struct SpecialChar { bool Matched; };

public:
    MenuController(MenuData& menu_data);
    virtual ~MenuController() {};

    /**
     * @brief Processes character accordingly
     *
     * Designed so that a child class can overwrite how it processes special characters and can add new special characters
     */
    int ProcessChar(const wchar_t& c);

    /**
     * @brief Sets title using information of the current option list and based on current mode and permissions
     */
    virtual void SetTitle() = 0;

    /**
     * @brief Used to update status bar
     *
     * Used by log class, so don't log inside this function otherwise there will be infinite recursion
     */
    void UpdateStatus();

    /**
     * @brief If last command was directly from input, which is used for menu actions in parser class
     */
    bool WasInput() { return Was_Input_; }

protected:
    /**
     * @brief Processes characters that might cause program to exit
     */
    virtual PossibleExit ProcessPossibleExit_(const wchar_t& c) = 0;

    /**
     * @brief Processes special characters that correspond to a specific user action
     */
    virtual SpecialChar ProcessSpecialChars_(const wchar_t& c) = 0;

    /**
     * @brief Processes characters have not been processed by two functions above, either doing nothing because they are not printable, or printing them
     */
    virtual void ProcessDefaultChar_(const wchar_t& c) = 0;

    MenuData& Menu_Data_;
    bool Was_Input_ = false;
};
