#pragma once

#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <filesystem>
#include <cstdlib>

class Entry {
public:
    enum status { SUCCESS, FAILIURE, RUNNING } status;

protected:
    std::string str_;
    std::string script_path_;
    std::string data_;

    void UpdateStatus_(int exit_code) {
	switch (exit_code) {
	    case(0):
		status = SUCCESS;
		break;
	    default:
		status = FAILIURE;
	}
    }

    void RunScript_() {
	status = RUNNING;
	std::string command = script_path_ + " " + data_;
	int exit_code = WEXITSTATUS(std::system(command.c_str()));
	UpdateStatus_(exit_code);
    }

public:
    Entry(const std::string& str, const std::filesystem::path& script_path, const std::string& data)
	: str_(str), script_path_(script_path), data_(str)
    {
    }

    bool operator<(const Entry& other) const {
	return str_ < other.str_;
    }

    std::string GetString() const {
	return str_;
    }

    std::string GetData() const {
	return data_;
    }

    virtual void Execute() = 0;
    virtual std::string GetDisplayString() = 0;
};

class DefaultEntry: public Entry {
public:
    DefaultEntry(const std::string& str, const std::filesystem::path& script, const std::string data)
	: Entry(str, script, data)
    {
    }

    void Execute() override {
	RunScript_();
    }

    std::string GetDisplayString() override {
	return GetString();
    }
};

class OutputEntry: public Entry {
protected:
    std::string RunScriptGetOutput_() {
	status = RUNNING;

	std::string output;

	// Open the script for reading
	FILE* pipe = popen(script_path_.c_str(), "r");
	if (!pipe) {
	    std::cerr << "Failed to execute script: " << script_path_ << std::endl;
	    return "";
	}

	// Read the output of the script
	char buffer[128];
	output = "";
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
	    output += buffer;
	}

	// Close the pipe and get the exit code of the script
	int exit_code = pclose(pipe);
	UpdateStatus_(exit_code);

	return output;
    }

public:
    OutputEntry(const std::string& str, const std::filesystem::path& script, const std::string data)
	: Entry(str, script, data)
    {
    }

    void Execute() override {
	RunScript_();
    }

    std::string GetDisplayString() override {
	std::string output;

	RunScriptGetOutput_();
	return output;
    }
};

