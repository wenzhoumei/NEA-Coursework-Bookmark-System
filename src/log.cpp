#include "log.hpp"
#include "menu_tui.hpp"

enum LogColor Log::GetCurrentColorForMenu() {
    return Color_;
}
void Log::SetLogPath(std::filesystem::path log_path) {
    Log_Path_ = log_path;
    Log_Path_Set_ = true;
}

void Log::Time() {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    // Convert to a std::tm object to extract the date and time
    std::tm tm = *std::localtime(&time);

    // Create a wide string with the formatted date and time
    std::wstringstream wss;
    wss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
    std::wstring time_str = L"Time: " + wss.str();

    Entries_.push_front(time_str);
    Num_Added_Options_Session_++;
}

void Log::PrintSession() {
    for (auto entry: Entries_) {
	std::wcout << entry << std::endl;
    }
}

const std::wstring& Log::Peek() {
    return Entries_.back();
}

void Log::LogStream::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    // Add most recent
    Log_.Entries_.push_back(Os_.str());
    Log_.Num_Added_Options_Session_++;

    Log_.Color_ = Color_;

    // Remove least recent
    while (Log_.Entries_.size() > 200) {
	Log_.Entries_.pop_front();
	Log_.Num_Added_Options_Session_--;
    }

    bool menu_opened = Log_.Menu_TUI_ != nullptr && Log_.Menu_TUI_->GetMenuController() != nullptr;
    if (menu_opened) {
	Log_.Menu_TUI_->GetMenuController()->UpdateStatus();
    }

    if (Exit_After_) {
	if (menu_opened) { Log_.Menu_TUI_->Close(); }
	Log_.FlushSession();
	exit(Exit_Code_);
    }
}

Log::LogStream Log::Warning() {
    return LogStream(L"Warning", LogColor::Warning, *this);
}

Log::LogStream Log::Error(int exit_code) {
    return LogStream(L"Error", LogColor::Error, *this, true, exit_code);
}

Log::LogStream Log::Info() {
    return LogStream(L"Info", LogColor::Info, *this);
}

Log::LogStream Log::Debug() {
    return LogStream(L"Debug", LogColor::Debug, *this);
}

void Log::SetHistoryPath(std::filesystem::path history_path) {
    History_Path_ = history_path;
    History_Path_Set_ = true;
}

void Log::History(std::wstring option_string) {
    // Add the option_string to end of History deque
    History_.push_back(option_string);

    // If the history path is not set, exit early
    if (!History_Path_Set_) {
        return;
    }

    // Open the history file for writing
    std::wofstream history_file(History_Path_);
    if (!history_file) {
        // Unable to open the file, handle the error
        return;
    }

    // Write the contents of the history deque to the file
    for (const auto& entry : History_) {
        history_file << entry << std::endl;
    }

    // Close the history file
    history_file.close();
}

void Log::FlushSession() {
    Flush_(Log_Path_, Entries_, MAX_LOG_LINES_); 
    Flush_(History_Path_, History_, MAX_HISTORY_LINES_); 
}

void Log::Flush_(const std::filesystem::path& path, const std::deque<std::wstring>& entries, const int max_lines) {
    if (path.empty()) { return; }

    PrintSession();

    // Open the file in append mode
    std::wofstream file(path, std::ios::app);

    // Print the deque to the end of the file
    for (auto it = entries.begin(); it != entries.end(); it++) {
        file << *it << std::endl;
    }

    // Close the file
    file.close();

    // Remove oldest entries if the file has more than max_lines
    std::wifstream infile(path);
    int line_count = 0;
    std::wstring line;
    while (getline(infile, line)) {
        line_count++;
    }
    infile.close();
    int excess_lines = line_count - max_lines;
    if (excess_lines > 0) {
        // Open the file in read mode
        std::wifstream infile2(path);
        // Create a temporary file to store the trimmed contents
        std::wofstream outfile("temp.txt");
        int lines_removed = 0;
        while (getline(infile2, line)) {
            if (lines_removed < excess_lines) {
                lines_removed++;
                break;
            }
            outfile << line << std::endl;
        }
        infile2.close();
        outfile.close();
        // Replace the original file with the trimmed version
        remove(path.c_str());
        rename("temp.txt", path.c_str());
    }
}

void Log::SetMenuTUI(MenuTUI* menu_tui) {
    Menu_TUI_ = menu_tui;
    if (menu_tui != nullptr) {
	Menu_TUI_->GetMenuController()->UpdateStatus();
    }
}

Log& my::log = Log::Instance();
