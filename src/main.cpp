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

    Parser parser;

    if (!parser.Config_Directory.SetPath(config_file_path)) { return 1; };

    if (!parser.Config_Directory.Scripts_Retriever->Load()) { return 2; };
    if (!parser.Config_Directory.IdentifierExtension_To_Action_Retriever->Load()) { return 2; };

    parser.LoadScripts();
    if (!parser.LoadIdentifierExtensions()) { return 3; };

    parser.ExecuteOptionString(argument);
}
