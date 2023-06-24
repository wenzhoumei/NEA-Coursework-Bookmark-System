#include "log.hpp"

void Log::LoadLogPath(std::filesystem::path log_path) {
    Path_ = log_path;
    std::wifstream file(Path_, std::wios::in);
    
    if (file.is_open()) {
        std::wstring line;
        while (std::getline(file, line)) {
            Entries_.push_back(line);
        }
        file.close();
    }
}

void Log::Write(std::wstring str) {
    Entries_.push_front(str);
    FlushSession();
}

void Log::PrintSession() {
    std::cout << "--Log--" << std::endl;
    for (auto entry: Entries_) {
	std::wcout << entry << std::endl;
    }
}

void Log::FlushSession() {
    // Open the file in write mode
    std::wofstream file(Path_, std::wios::out);

    // Flush to file
    int count = std::min(MAX_LOG_LINES_, Entries_.size());
    for (int i = 0; i < count; i++) {
        file << *(Entries_.begin() + i) << std::endl;
    }

    // Close the file
    file.close();
}

Log my::log;
Log my::history;
