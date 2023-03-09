#pragma once
#include <stddef.h>

/** Deals with the logic of going up and down
 *  In future, can inherit to implement circular navigation
 **/
class SelectedOptionPositionController {
public:
    void Down(const size_t& max, size_t& selected_option_position) {
	if (max == 0) { return; }
	if (selected_option_position == max) { return; }

	selected_option_position += 1;
    }

    void Up(const size_t& max, size_t& selected_option_position) {
	if (selected_option_position == 0) { return; }

	selected_option_position -= 1;
    }
};
