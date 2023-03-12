#pragma once
#include "component_controller.hpp"

/**
 * @brief Handles events pertaining to selected option position
 */
class SelectedOptionPositionController: public ComponentController {
public:
    SelectedOptionPositionController(MenuData* menu_data);

    /**
     * @brief Moves selected option position down if possible
     */
    void Down();

    /**
     * @brief Moves selected option position up if possible
     */
    void Up();

    /**
     * @brief Moves selected option position to start
     */
    void Reset();

private:
    /**
     * @brief Gets max, which changes depending on mode
     */
    size_t Max_();

    /**
     * @brief All changes of selected option funnelled through here, which updates Changed struct and input depnding on mode
     */
    void ChangePos(size_t new_position);
};
