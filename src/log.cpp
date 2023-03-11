#include "log.hpp"
#include "menu_tui.hpp"

void Log::LogStream::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    // Add most recent
    log_.entries_.push_back(os_.str());
    log_.NumAddedOptionsSession++;

    log_.color_ = color_;

    // Remove least recent
    while (log_.entries_.size() > 200) {
	log_.entries_.pop_front();
	log_.NumAddedOptionsSession--;
    }

    bool menu_opened = log_.menu_tui_ != nullptr;
    if (menu_opened) {
	log_.menu_tui_->GetMenuController()->UpdateStatus();
    }

    if (exit_after_) {
	if (menu_opened) { log_.menu_tui_->Close(); }
	exit(exit_code_);
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

void Log::FlushSession() {
    if (!log_path_set_) { return; }

    PrintSession();
    
    // Open the file in append mode
    std::wofstream file(log_path_, std::ios::app);
    
    // Print the deque to the end of the file
    for (auto it = entries_.rbegin(); it != entries_.rend(); it++) {
        file << *it << std::endl;
    }
    
    // Close the file
    file.close();
    
    // Remove oldest entries if the file has more than 200 lines
    std::wifstream infile(log_path_);
    int lineCount = 0;
    std::wstring line;
    while (getline(infile, line)) {
        lineCount++;
    }
    infile.close();
    int excessLines = lineCount - MAX_LINES;
    if (excessLines > 0) {
        // Open the file in read mode
        std::wifstream infile2(log_path_);
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
        remove(log_path_.c_str());
        rename("temp.txt", log_path_.c_str());
    }
}

void Log::SetMenuTUI(MenuTUI* menu_tui) {
    menu_tui_ = menu_tui;
    if (menu_tui != nullptr) {
	menu_tui_->GetMenuController()->UpdateStatus();
    }
}

Log& my::log = Log::Instance();
