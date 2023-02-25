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

    if (!parameter_processor.IsNumParametersValid() || !parameter_processor.IsPathValid()) {
	std::cerr << "Usage: " << argv[0] << " option_string config_directory\n";
	return 1;
    }

    std::string argument = parameter_processor.GetOptionString();

    Settings& settings = Settings::Instance();
    settings.Initialize(parameter_processor.GetOptionString());

    std::cout << "The argument you provided is: " << argument << "\n";

    // Rest of your program logic goes here

    return 0;
}
