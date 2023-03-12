#pragma once
#include "component_controller.hpp"

class TitleController: public ComponentController {
public:
    TitleController(MenuData* menu_data);

    /**
     * Sets text of title
     */
    void SetText(const std::wstring& new_title);
};
