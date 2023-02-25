#pragma once

#include "option.hpp"
#include "settings.hpp"

class OptionFactory {
private:
    OptionFactory(): settings_(Settings::Instance()) {}; // Private constructor to prevent instantiation
    OptionFactory(const OptionFactory&) = delete; // Delete copy constructor
    OptionFactory& operator=(const OptionFactory&) = delete; // Delete assignment operato

    Settings& settings_;

public:
    static OptionFactory& Instance()
    {
	static OptionFactory INSTANCE;
	return INSTANCE;
    }

    Option* Create(const std::wstring& option_str);
};
