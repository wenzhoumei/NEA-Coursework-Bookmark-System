#include "menu_data.hpp"

std::stack<std::pair<std::wstring, std::wstring>> MenuData::History;

MenuData::MenuData(OptionList* option_list)
    : Option_List(std::move(option_list))
{
}

void MenuData::Changed::SetAll() {
    Title = Input = Option_List = Selected = Status_Log = true;
}

void MenuData::Changed::SetNone() {
    Title = Input = Option_List = Selected = Status_Log = false;
}

std::wstring MenuData::SelectedOptionString() const {
    return Option_List->OptionStringAt(Selected_Option_Position);
}

std::wstring MenuData::SelectedName() const {
    return Option_List->NameAt(Selected_Option_Position);
}

std::wstring MenuData::SelectedData() const {
    return Option_List->DataAt(Selected_Option_Position);
}

bool MenuData::IsSearchListEmpty() const {
    return Option_List->SearchedSize() == 0;
}
