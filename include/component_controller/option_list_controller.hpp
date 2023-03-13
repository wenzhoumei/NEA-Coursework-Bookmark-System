#pragma once

#include "component_controller/component_controller.hpp"
#include "component_controller/selected_option_position_controller.hpp"

/**
 * @brief Connects corresponding functions in OptionList class to interact with other components of menu
 */
class OptionListController: public ComponentController {
public:
    OptionListController(MenuData* menu_data, SelectedOptionPositionController& selected_option_position_controller);

    /**
     * @brief Searches MenuData::Option_List_ with current input, called every time input is modified in search mode
     */
    void Search();

    /**
     * @brief Adds current input to option list
     */
    void Add();

    /**
     * @brief Removes selected option from option list if there is one
     */
    void Remove();

    /**
     * @brief Edits name or data of option list depending on which one is selected, if it already exists, swap the positions of the two options
     */
    void Update();

    /**
     * @brief Inserts current input at selected position (on screen)
     */
    void Insert();

protected:
    SelectedOptionPositionController& selected_position_controller_;
};
