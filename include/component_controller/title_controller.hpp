#pragma once
#include "component_controller.hpp"

/**
 * @brief Handles changing of title
 */
class TitleController: public ComponentController {
public:
    TitleController(MenuData* menu_data);

    /**
     * @brief Sets text of title
     */
    void SetText(const std::wstring& new_title);
};
