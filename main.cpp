#include "menu.hpp"
#include <locale>

int main() {
    /*
    FileVectorEntryList v(L"hello", "/home/wenzhou/Entries/files/hello.txt");
    v.PrintAllEntries();
    v.PrintSearchedEntries();
    */

    std::locale::global(std::locale(""));

    Menu menu;
    std::wcout << menu.Run() << std::endl;
    return 0;
}

/*
int main() {
    FileEntryList f("/home/wenzhou/Entries/config/hello.txt");
    f.UpdateEntry(EntryProcessor::CreateEntry("k"));
    f.PrintEntries();
    DirectoryEntryList d("/home/wenzhou/Entries/config/extensions");

    d.AddEntry(EntryProcessor::CreateEntry("when"));

    ExecutableDirectoryEntryList d2("/home/wenzhou/Entries/config/scripts");
    d2.PrintEntries();
}
*/
