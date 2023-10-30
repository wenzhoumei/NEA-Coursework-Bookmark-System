#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <deque>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <stack>
#include <queue>

class Log {
public:
    bool LoadLogPath(std::filesystem::path log_path);
    void Write(std::wstring text);

    void PrintSession();
    void FlushSession();

    std::wstring GetStatusMessage() { return Status_Message_; }

protected:
    void Flush_(const std::filesystem::path& path, const std::deque<std::wstring>& entries, int max_lines);

    std::deque<std::wstring> Entries_; ///< Stores strings of all of the flushed log entries
				       
    std::wstring Status_Message_ = L"Start";

    std::filesystem::path Path_; ///< Contains path of log file
    const size_t MAX_LOG_LINES_ = 20; ///< Max number of lines of entries to be stored inside log file
};

namespace my {
    extern Log log; ///< More compact way of referencing instance of singleton
    extern Log history; ///< More compact way of referencing instance of singleton
}
