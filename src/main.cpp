#include "config_directory.hpp"
#include "parser.hpp"
#include "parameter_processor.hpp"
#include "log.hpp"
#include <functional>


int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));

    const std::filesystem::path config_file_path = "/home/wenzhou/Entries/files";

    ConfigDirectory& config_directory = ConfigDirectory::Instance();
    config_directory.Initialize(config_file_path);

    Parser& parser = Parser::Instance();

    Log& log = Log::Instance();
    log.Time();
    std::atexit([]() { Log::Instance().FlushSession(); });

    //std::atexit([]() { Log::Instance().PrintSession(); });

    ParameterProcessor parameter_processor(argc, argv);

    if (!parameter_processor.IsNumParametersValid()) {
	log.Error(1) << "Usage - " << argv[0] << " option_string";
    }

    std::wstring argument = parameter_processor.GetOptionString();


    parser.LoadScripts();

    if (!parser.LoadIdentifierExtensions()) { return 3; };

    parser.ExecuteOptionString(argument);
}
