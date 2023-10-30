#include "config_directory.hpp"
#include "parser.hpp"
#include "parameter_processor.hpp"
#include "log.hpp"
#include <functional>

void ExitProgram(int exit_code) {
    my::log.PrintSession();
    my::log.FlushSession();

    exit(exit_code);
}

int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));

    const char* config_file_path_cstr = std::getenv("MY_MENY_CONFIG");

    // Get config directory name
    std::filesystem::path config_file_path;
    if (config_file_path_cstr == nullptr) {
	config_file_path = std::getenv("HOME");
	config_file_path /= ".config/my_menu";
    } else {
	config_file_path = std::filesystem::path(config_file_path_cstr);
    }

    if (!std::filesystem::is_directory(config_file_path)) {
	my::log.Write(L"ConfigLoadError: Please add configuration directory into $HOME/.config/my_menu");
	ExitProgram(1);
    }

    ConfigDirectory& config_directory = ConfigDirectory::Instance();

    if (!config_directory.Initialize(config_file_path)) {
	ExitProgram(1);
    }

    std::atexit([]() { my::log.PrintSession(); my::log.FlushSession(); });

    if (!my::log.LoadLogPath(config_directory.GetLogFilePath())) {
	my::log.Write(L"ConfigLoadError: Please add configuration directory into $HOME/.config/my_menu");
	ExitProgram(1);
    }
    my::log.LoadLogPath(config_directory.GetHistoryFilePath());

    Parser& parser = Parser::Instance();

    ParameterProcessor parameter_processor(argc, argv);

    if (!parameter_processor.IsNumParametersValid()) {
	my::log.Write(L"Invalid parameters");
	ExitProgram(1);
    }

    std::wstring argument = parameter_processor.GetOptionString();

    if (argument == L"") {
	argument = L"root@bmk";
    }

    parser.LoadScripts();

    if (!parser.LoadIdentifierExtensions()) { return 3; };

    parser.ExecuteOptionString(argument);

}
