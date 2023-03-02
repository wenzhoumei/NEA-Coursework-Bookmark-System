#include <iostream>

#include "file_option_list.hpp"
#include "option_list.hpp"

void PrintOptionList(const OptionList& option_list) {
    std::cout << "Option list:\n";
    std::cout << "----------------\n";

    for (const std::string& name : option_list.SubList(0, option_list.Size())) {
        std::cout << name << ">" << option_list.DataAt(name) << std::endl;
    }
}

int main() {
    FileOptionList f("/home/wenzhou/Entries/files/hello.txt");
    f.LoadOptions();
    PrintOptionList(f);
}

/*
int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));

    ParameterProcessor parameter_processor(argc, argv);

    if (!parameter_processor.IsNumParametersValid()) {
	std::cerr << "Usage: " << argv[0] << " option_string config_directory\n";
	return 1;
    }

    std::string argument = parameter_processor.GetOptionString();

    ConfigDirectory& config_directory = ConfigDirectory;
    config_directory.SetPath("/home/wenzhou/Entries/files");

    ConfigRetriever config_retriever = ConfigRetriever(config_directory);

    if (!config_retriever.LoadScripts()) {
    }

    if (!config_retriever.LoadOptionLists()) {
    }

    if (!config_retriever.LoadIdentifierExtensions()) {
    }

    ConfigWriter& config_writer = ConfigWriter::Instance();

    Settings& settings = Settings::Instance();
    if (!config(config_directory)) {
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
*/
