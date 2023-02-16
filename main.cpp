#include <string>
#include "entry_list.hpp"
#include "entry_processor.hpp"


int main() {
    FileEntryList f("/home/wenzhou/Entries/config/hello.txt");
    f.UpdateEntry(EntryProcessor::CreateEntry("k"));
    f.PrintEntries();
    DirectoryEntryList d("/home/wenzhou/Entries/config/extensions");

    d.AddEntry(EntryProcessor::CreateEntry("when"));

    ExecutableDirectoryEntryList d2("/home/wenzhou/Entries/config/scripts");
    d2.PrintEntries();
}
