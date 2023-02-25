#include <string>
#include <vector>
#include <memory>
#include "option.hpp"

class MenuData {
    std::unique_ptr<Option> Title;

    std::string Input;
    size_t CursorPosition;

    std::vector<std::unique_ptr<Option>> SearchedOptions;
    size_t SelectedOptionPosition;

    enum { TITLE, OPTION, STATUS_LOG } Selected;

    std::vector<std::string> StatusLog;
};

class StatusLogController;
// Manipulates components of menu data class
class ComponentController {
public:
    ComponentController(MenuData& menu_data, StatusLogController& status_log_controller): menu_data_(menu_data), status_log_controller_(status_log_controller) {}
protected:
    MenuData& menu_data_;
    StatusLogController& status_log_controller_;
};

class StatusLogController: public ComponentController {
public:
    StatusLogController(MenuData& menu_data): ComponentController(menu_data, *this) {}

    template<typename T> StatusLogController& operator<<(const T& value);
} ;

class TitleController: public ComponentController {
public:
    TitleController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}

    void SetText(const std::wstring& text);
};

class SelectedOptionPositionController: public ComponentController {
public:
    SelectedOptionPositionController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}
};

class OptionsController: public ComponentController {
public:
    OptionsController(MenuData& menu_data, StatusLogController& status_log_controller, SelectedOptionPositionController& selected_option_position_controller): ComponentController(menu_data, status_log_controller), selected_position_controller_(selected_option_position_controller) {}
    void Search();

protected:
    SelectedOptionPositionController& selected_position_controller_;
};

class CursorPositionController: public ComponentController {
public:
    CursorPositionController(MenuData& menu_data, StatusLogController& status_log_controller): ComponentController(menu_data, status_log_controller) {}

    // Move cursor
    void Right();
    void Left();
};

class InputController: public ComponentController {
public:
    InputController(MenuData& menu_data, StatusLogController& status_log_controller, CursorPositionController& cursor_position_controller, OptionsController& options_controller): ComponentController(menu_data, status_log_controller), cursor_position_controller_(cursor_position_controller), options_controller_(options_controller) {}

    void SetText(const std::wstring& text);
    void PopChar();
    void AddChar();

protected:
    CursorPositionController& cursor_position_controller_;
    OptionsController& options_controller_;
};

class MenuController {
public:
    MenuController(MenuData& menu_data, const MenuOption& menu_option) {
	status_log_controller_ = new StatusLogController(menu_data);
	title_controller_ = new TitleController(menu_data, *status_log_controller_);

	selected_option_position_controller_ = new SelectedOptionPositionController(menu_data, *status_log_controller_);
	options_controller_ = new OptionsController(menu_data, *status_log_controller_, *selected_option_position_controller_);

	cursor_position_controller_ = new CursorPositionController(menu_data, *status_log_controller_);
	input_controller_ = new InputController(menu_data, *status_log_controller_, *cursor_position_controller_, *options_controller_);
    }

    StatusLogController* status_log_controller_;
    TitleController* title_controller_;
    CursorPositionController* cursor_position_controller_;
    InputController* input_controller_;
    SelectedOptionPositionController* selected_option_position_controller_;
    OptionsController* options_controller_;
};

// Handles display of MenuData
// Recieves events and execute appropriate Controller function
class MenuTUI {
public:
    MenuTUI(MenuOption menu_option) {
        menu_data_ = new MenuData();
        menu_controller_ = new MenuController(*menu_data_, menu_option);
    }

    ~MenuTUI(); // Delete menu controllers and data

    void Display(); // Display menu_data_

    std::wstring Input(); // Get character and handle with appropriate controller function

private:
    MenuData* menu_data_;

    MenuController* menu_controller_;
};
