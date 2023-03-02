#include "config_directory.hpp"
#include "parser.hpp"

int main() {
    std::filesystem::path file_path = "/home/wenzhou/Entries/files";
    ConfigDirectory config_directory;

    if (!config_directory.SetPath(file_path)) { return 2; };

    Parser parser(config_directory);

    if (!parser.Scripts_Retriever.Load()) { return 2; };
    if (!parser.IdentifierExtension_To_Script_Retriever.Load()) { return 2; };

    parser.LoadScripts();
    if (!parser.LoadIdentifierExtensions()) { return 3; };
}
