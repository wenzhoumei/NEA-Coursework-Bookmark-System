#include "log.hpp"
#include "config_directory.hpp"

/*
void Log::FlushSession() {
    PrintSession();

    std::filesystem::path file_path = ConfigDirectory::Instance().GetLogFilePath();

    const int max_lines = 200;

    // Open the file for reading and count the number of lines
    std::wstd::ifstream count_file(file_path);
    int num_lines = std::count(std::istreambuf_iterator<wchar_t>(count_file), std::istreambuf_iterator<wchar_t>(), L'\n');
    count_file.close();

    // Open the file for writing in text mode and move the file pointer to the end
    std::wstd::ofstream file(file_path, std::ios::app);
    file.seekp(0, std::ios::end);

    // If the file already has max_lines, remove the oldest line from the beginning
    if (num_lines >= max_lines) {
	std::wstd::ifstream read_file(file_path);
	std::wstd::string line;
	std::getline(read_file, line);
	read_file.close();
    }

    // Write the entries to the file
    for (auto entry: entries_) {
	file << entry << std::endl;
    }

    // Close the file
    file.close();

    // Clear the entries vector
    entries_.clear();
}
*/

void Log::FlushSession() {
    const int MAX_LINES = 20;

    std::filesystem::path file_path = ConfigDirectory::Instance().GetLogFilePath();

    // Reverse the vector
    reverse(entries_.begin(), entries_.end());

    PrintSession();
    
    // Open the file in append mode
    std::wofstream file(file_path, std::ios::app);
    
    // Append the vector to the end of the file
    for (const auto& entry : entries_) {
        file << entry << std::endl;
    }
    
    // Close the file
    file.close();
    
    // Remove oldest entries if the file has more than 200 lines
    std::wifstream infile(file_path);
    int lineCount = 0;
    std::wstring line;
    while (getline(infile, line)) {
        lineCount++;
    }
    infile.close();
    int excessLines = lineCount - MAX_LINES;
    if (excessLines > 0) {
        // Open the file in read mode
        std::wifstream infile2(file_path);
        // Create a temporary file to store the trimmed contents
        std::wofstream outfile("temp.txt");
        int linesRemoved = 0;
        while (getline(infile2, line)) {
            if (linesRemoved < excessLines) {
                linesRemoved++;
                continue;
            }
            outfile << line << std::endl;
        }
        infile2.close();
        outfile.close();
        // Replace the original file with the trimmed version
        remove(file_path.c_str());
        rename("temp.txt", file_path.c_str());
    }
}
