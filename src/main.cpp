#include <iostream>
#include <string>
#include <functional>
#include <sstream>

#include "parameter_processor.hpp"
#include "settings.hpp"

#include <filesystem>

int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));

    ParameterProcessor parameter_processor(argc, argv);

    if (!parameter_processor.IsNumParametersValid()) {
	std::cerr << "Usage: " << argv[0] << " option_string config_directory\n";
	return 1;
    }

    std::string argument = parameter_processor.GetOptionString();

    std::filesystem::path config_directory = "/home/wenzhou/Entries/files";
    Settings& settings = Settings::Instance();
    if (!settings.Initialize(config_directory)) {
	std::cerr << "Invalid config directory path" << std::endl;
	return 2;
    }

    if (!settings.LoadScripts()) {
	std::cerr << "Failed to load action names" << std::endl;
	return 2;
    }

    if (!settings.LoadIdentifierExtensions()) {
	std::cerr << "Failed to load identifier extensions" << std::endl;
	return 2;
    }

    std::cout << "The argument you provided is: " << argument << "\n";

    return 0;
}
