#include "config_directory.hpp"
#include "parser.hpp"
#include "parameter_processor.hpp"
#include "log.hpp"
#include <functional>


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
	my::log.Error(ExitCode::ConfigLoadError) << "Please add configuration directory into $HOME/.config/my_menu" << std::endl;
    }

    ConfigDirectory& config_directory = ConfigDirectory::Instance();
    config_directory.Initialize(config_file_path);

    std::atexit([]() { my::log.FlushSession(); });

    my::log.Time();
    my::log.SetLogPath(config_directory.GetLogFilePath());
    my::log.SetHistoryPath(config_directory.GetHistoryFilePath());

    Parser& parser = Parser::Instance();

    ParameterProcessor parameter_processor(argc, argv);

    if (!parameter_processor.IsNumParametersValid()) {
	my::log.Error(1) << "Usage - " << argv[0] << " option_string" << std::endl;
    }

    std::wstring argument = parameter_processor.GetOptionString();

    if (argument == L"") {
	std::wstring workspace = config_directory.GetWorkspaceFileContents();
	if (workspace == L"") {
	    argument = L"root@bmk";
	} else {
	    argument = workspace;
	}
    }

    parser.LoadScripts();

    if (!parser.LoadIdentifierExtensions()) { return 3; };

    parser.ExecuteOptionString(argument);
}
