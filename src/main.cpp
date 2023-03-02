#include "config_directory.hpp"
#include "parser.hpp"
#include "parameter_processor.hpp"

int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));

    ParameterProcessor parameter_processor(argc, argv);

    if (!parameter_processor.IsNumParametersValid()) {
	std::cerr << "Usage: " << argv[0] << " option_string\n";
	return 1;
    }

    std::string argument = parameter_processor.GetOptionString();

    std::filesystem::path config_file_path = "/home/wenzhou/Entries/files";

    ConfigDirectory config_directory;
    if (!config_directory.SetPath(config_file_path)) { return 2; };

    Parser parser(config_directory);

    if (!parser.Scripts_Retriever.Load()) { return 2; };
    if (!parser.IdentifierExtension_To_Action_Retriever.Load()) { return 2; };

    parser.LoadScripts();
    if (!parser.LoadIdentifierExtensions()) { return 3; };

    parser.ExecuteOptionString(argument);
}
