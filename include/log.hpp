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

#include "exit_code.hpp"

/**
 * @brief Identifies color that is associated with each of the log types, which is used to create INIT_COLOR_PAIR in menu_view.hpp
 */
enum LogColor {
    Info = 1, 
    Warning = 2, 
    Error = 3, 
    Debug = 4, 

    Normal = 5, //
};

class Log;
namespace my {
    extern Log& log; ///< More compact way of referencing instance of singleton
}

/**
 * @brief Custom singleton log class that writes text streamed into it to stdout, menu status bar and the log file if they are loaded
 *
 * Different types of log
 * Error: Program can't continue functioning, logs text and exits
 * Warning: Action may lead to undefined behavior
 * Info: General information to communicate with user
 * Debug: Used for debugging
 */
class MenuTUI;
class Log {
    Log() {}; ///< Private constructor to prevent instantiation
    Log(const Log&) = delete; ///< Delete copy constructor
    Log& operator=(const Log&) = delete; ///< Delete assignment operator
							     
    /**
     * @brief Nested class representing the stream used for logging output.
     */
    class LogStream {
	public:
	    /**
	     * @param prefix The prefix that identifies to the user the type of log message, "" if no prefix
	     * @param color The color to use for the log message.
	     * @param log Reference to the `Log` object that creates this stream
	     * @param exit_after Indicates whether the program should exit after logging the message.
	     * @param exit_code Exit code to use if the program exits after logging the message.
	     */
	    LogStream(const std::wstring& prefix, enum LogColor color, Log& log, bool exit_after=false, int exit_code=0)
		: Color_(color), Log_(log), Exit_After_(exit_after), Exit_Code_(exit_code)
	    {
		if (exit_after) {
		    Os_ << prefix << " (" << exit_code << ") : ";
		} else {
		    Os_ << prefix << ": ";
		}
	    }

	    /**
	     * @brief Appends text to current buffer
	     *
	     * @tparam T The type of the value to write.
	     * @param value The value to write to the log stream.
	     * @return A reference to the LogStream object to allow for chaining of the output
	     */
	    template<typename T>
		LogStream& operator<<(const T& value) {
		    Os_ << value;
		    return *this;
		}

	    /**
	     * @brief Catches std::endl, indicating to flush Os_
	     * 
	     * @param manipulator To catch std::endl
	     */
	    void operator<<(std::ostream& (*manipulator)(std::ostream&));

	private:
	    std::wostringstream Os_; ///< The stream buffer used to store the log message.
	    enum LogColor Color_; ///< The color to use for the log message.
	    Log& Log_; ///< A reference to the `Log` object that is using this stream.
	    int Exit_After_; ///< Indicates whether the program should exit after logging the message.
	    int Exit_Code_; ///< The exit code to use if the program exits after logging the message.
    };

public:
    /**
     * @brief Access point to only instance of class
     *
     * @return Meyer's singleton
     */
    static Log& Instance()
    {
	static Log INSTANCE;
	return INSTANCE;
    }

    /**
     * @brief Gets the current color for the menu.
     *
     * @return The current color for the menu.
     */
    enum LogColor GetCurrentColorForMenu();

    /**
     * @brief Sets the path to the log file.
     *
     * @param log_path The path to the log file.
     */
    void SetLogPath(std::filesystem::path log_path);

    /**
     * @brief Sets the path to the history file.
     *
     * @param history_path The path to the history file.
     */
    void SetHistoryPath(std::filesystem::path history_path);

    /**
     * @brief Sets the MenuTUI object for the log.
     *
     * @param menu_tui The MenuTUI object to close when exiting and for its status bar
     */
    void SetMenuTUI(MenuTUI* menu_tui);

    /**
     * @brief Outputs the current time to the log.
     */
    void Time();

    /**
     * @brief Creates a LogStream object for a warning message.
     *
     * @return A LogStream object for a warning message.
     */
    LogStream Warning();

    /**
     * @brief Creates a LogStream object for an error message with an exit code.
     *
     * @param exit_code The exit code for the error message.
     * @return A LogStream object for an error message with an exit code.
     */
    LogStream Error(int exit_code);

    /**
     * @brief Creates a LogStream object for an informational message.
     *
     * @return A LogStream object for an informational message.
     */
    LogStream Info();

    /**
     * @brief Creates a LogStream object for a debug message.
     *
     * @return A LogStream object for a debug message.
     */
    LogStream Debug();

				       
    /**
     * @brief Adds to command to history and flushes
     */
    void History(std::wstring option_string);

    /**
     * @brief Prints the session to the log.
     */
    void PrintSession();

    /**
     * @brief Gets the next message from the log without removing it.
     *
     * @return The next message from the log without removing it.
     */
    const std::wstring& Peek();

    /**
     * @brief Flushes the current session to the log.
     */
    void FlushSession();

private:
    void Flush_(const std::filesystem::path& path, const std::deque<std::wstring>& entries, int max_lines);

    std::deque<std::wstring> Entries_; ///< Stores strings of all of the flushed log entries
				       
    std::filesystem::path Log_Path_; ///< Contains path of log file
    bool Log_Path_Set_ = false; ///< Indicates if Log_Path_ has been set
    std::deque<std::wstring> History_; ///< Stores strings of all of the flushed log entries
    const size_t MAX_LOG_LINES_ = 20; ///< Max number of lines of entries to be stored inside log file
				
    std::filesystem::path History_Path_; ///< Contains path of history file
    bool History_Path_Set_ = false; ///< Indicates if History_File_ has been set
    static const size_t MAX_HISTORY_LINES_ = 5; ///< Max number of lines of entries to be stored inside log file

    MenuTUI* Menu_TUI_ = nullptr; ///< Pointer to Menu_TUI_ to use its status bar and exit gracefully, calling menu_view's destructor

    enum LogColor Color_ = LogColor::Info; ///< Indicates color of most recently flushed entry for status bar
    int Num_Added_Options_Session_ = 0; ///< Number of options added in the current session
};
