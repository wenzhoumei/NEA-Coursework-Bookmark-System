#include "entry.hpp"
#include "entry_list.hpp"
#include "vector_file_entry_list.hpp"
#include "menu.hpp"

int main() {
    /*
    VectorFileEntryList v("/home/wenzhou/Entries/files/hello.txt");

    v.Search("");
    v.PrintSearchedEntries();
    std::cout << v.SearchedSize() << std::endl;

    v.Search("t 4");
    v.PrintSearchedEntries();
    std::cout << v.SearchedSize() << std::endl;
    */

    Menu menu("title", std::make_unique<VectorFileEntryList>(VectorFileEntryList("/home/wenzhou/Entries/files/hello.txt")));
    int choice = menu.run();
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
