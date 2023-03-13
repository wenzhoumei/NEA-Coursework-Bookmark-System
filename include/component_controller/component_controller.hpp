#pragma once
#include "menu_data.hpp"

/**
 * @brief Abstract parent class for all component controllers
 *
 * Used to encapsulate and group different functions run in the MenuController interface according to what is the main part of the menu they are editing in MenuData
 * The change of there respective data can change trigger the changing of different parts of MenuData and also updates MenuData::Changed struct
 * In the future, can inherit one of these classes to allow change of functionality at runtime of a certain component, like making selected option position circular
 */
class ComponentController {
public:
    ComponentController(MenuData* menu_data)
	: Menu_Data_(menu_data) {}
protected:
    MenuData* Menu_Data_;
};
