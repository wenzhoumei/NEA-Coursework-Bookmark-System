#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <algorithm>

class Log {
    Log() {}; // Private constructor to prevent instantiation
    Log(const Log&) = delete; // Delete copy constructor
    Log& operator=(const Log&) = delete; // Delete assignment operato
							     
    class LogStream {
    public:
	LogStream(const std::wstring& prefix, Log& log, bool exit_after=false, int exit_code=0)
	    : log_(log), exit_after_(exit_after), exit_code_(exit_code)
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

	~LogStream() {
	    log_.entries_.insert(log_.entries_.begin(), os_.str());
	    log_.NumAddedOptionsSession++;

	    while (log_.entries_.size() > 200) {
		log_.entries_.pop_back();
		log_.NumAddedOptionsSession--;
	    }

	    log_.Changed = true;

	    if (exit_after_) { exit(exit_code_); }
	}

    private:
	std::wostringstream os_;
	Log& log_;
	int exit_after_;
	int exit_code_;
    };

    std::vector<std::wstring> entries_;

public:
    bool Changed = false;

    int NumAddedOptionsSession = 0;

    static Log& Instance()
    {
	static Log INSTANCE;
	return INSTANCE;
    }

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

	while (entries_.size() > 200) {
	    entries_.pop_back();
	    NumAddedOptionsSession--;
	}

	Changed = true;
    }

    LogStream Warning() {
        return LogStream(L"Warning", *this);
    }

    LogStream Error(int exit_code) {
        return LogStream(L"Error", *this, true, exit_code);
    }

    LogStream Info() {
        return LogStream(L"Info", *this);
    }

    LogStream Debug() {
        return LogStream(L"Debug", *this);
    }

    void PrintSession() {
	for (auto entry: entries_) {
	    std::wcout << entry << std::endl;
	}
    }

    void FlushSession();
};
