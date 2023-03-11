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

enum LogColor {
    Info = 1, 
    Warning = 2, 
    Error = 3, 
    Debug = 4, 
    Normal = 5, 
};

class MenuTUI;
class Log {
    Log() {}; // Private constructor to prevent instantiation
    Log(const Log&) = delete; // Delete copy constructor
    Log& operator=(const Log&) = delete; // Delete assignment operato
							     
    class LogStream {
    public:
	LogStream(const std::wstring& prefix, enum LogColor color, Log& log, bool exit_after=false, int exit_code=0)
	    : color_(color), log_(log), exit_after_(exit_after), exit_code_(exit_code)
	{
	    if (exit_after) {
		os_ << prefix << " (" << exit_code << ") : ";
	    } else {
		os_ << prefix << ": ";
	    }
	}

	template<typename T>
	LogStream& operator<<(const T& value) {
	    os_ << value;
	    return *this;
	}

	// Define a custom std::ostream& operator for std::endl
	void operator<<(std::ostream& (*manipulator)(std::ostream&));

    private:
	std::wostringstream os_;
	enum LogColor color_;
	Log& log_;
	int exit_after_;
	int exit_code_;
    };

    // back is most recent
    std::deque<std::wstring> entries_;

    std::filesystem::path log_path_;
    bool log_path_set_ = false;

    MenuTUI* menu_tui_;

    enum LogColor color_ = LogColor::Info;
public:
    const size_t MAX_LINES = 20;

    int NumAddedOptionsSession = 0;

    static Log& Instance()
    {
	static Log INSTANCE;
	return INSTANCE;
    }

    enum LogColor GetCurrentColorForMenu() {
	return color_;
    }
    void SetLogPath(std::filesystem::path log_path) {
	log_path_ = log_path;
	log_path_set_ = true;
    }

    void SetMenuTUI(MenuTUI* menu_tui);

    void Time() {
	// Get the current time
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	// Convert to a std::tm object to extract the date and time
	std::tm tm = *std::localtime(&time);

	// Create a wide string with the formatted date and time
	std::wstringstream wss;
	wss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
	std::wstring time_str = L"Time: " + wss.str();

	entries_.insert(entries_.begin(), time_str);
	NumAddedOptionsSession++;

	while (entries_.size() > MAX_LINES) {
	    entries_.pop_back();
	    NumAddedOptionsSession--;
	}
    }

    LogStream Warning();
    LogStream Error(int exit_code);
    LogStream Info();
    LogStream Debug();

    void PrintSession() {
	for (auto entry: entries_) {
	    std::wcout << entry << std::endl;
	}
    }

    const std::wstring& Peek() {
	return entries_.back();
    }

    void FlushSession();
};

namespace my {
    extern Log& log;
}
