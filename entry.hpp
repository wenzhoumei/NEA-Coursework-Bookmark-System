#pragma once

#include <string>
#include <filesystem>
#include <iostream>

class Entry {
public:
    enum status { SUCCESS, FAILIURE, RUNNING } status;

    void UpdateStatus(int exit_code) {
	switch (exit_code) {
	    case(0):
		status = SUCCESS;
		break;
	    default:
		status = FAILIURE;
	}
    }

    bool operator==(const Entry& other) const {
	return GetName() == other.GetName();
    }

    bool operator<(const Entry& other) const {
	return GetName() < other.GetName();
    }

    std::string GetString() const {
	if (GetData() == "") { return GetName(); }
	else { return GetName() + '\t' + GetData(); }
    }

    const bool DataEditable = false;
    const bool NameEditable = false;
    const bool Executable = false;

    virtual std::string GetData() const = 0;
    virtual bool SetData(std::string data) = 0;

    virtual std::string GetName() const = 0;
    virtual bool SetName(std::string name) = 0;

    virtual void Execute() = 0;
    virtual std::string GetDisplayString() = 0;
};

class DefaultEntry: public Entry {
protected:
    std::string name_;
    std::string script_path_;
    std::string data_;

    void RunScript_() {
	status = RUNNING;
	std::string command = script_path_ + " " + data_;
	int exit_code = WEXITSTATUS(std::system(command.c_str()));
	UpdateStatus(exit_code);
    }

public:
    const bool DataEditable = true;
    const bool NameEditable = true;
    const bool Executable = true;

    DefaultEntry(const std::string& name, const std::filesystem::path& script_path, const std::string& data)
	: name_(name), script_path_(script_path), data_(data)
    {
    }

    std::string GetData() const override { return data_; }
    bool SetData(std::string data) override {
	data_ = data;
	return true;
    };

    std::string GetName() const override { return name_; }
    bool SetName(std::string name) override {
	name_ = name;
	return true;
    };

    void Execute() override {
	RunScript_();
    }

    std::string GetDisplayString() override {
	return name_;
    }
};

class UnprocessedEntry: public Entry {
private:
    std::string SplitByFirst_(std::string& str, char del) {
	size_t first_del_pos = str.find(del);
	if (first_del_pos == std::string::npos) {
	    // Delimiter not found, return empty string
	    return "";
	} else {
	    // Split string into two parts
	    std::string first_part = str.substr(0, first_del_pos);
	    std::string end_part = str.substr(first_del_pos + 1);

	    str = first_part;
	    return end_part;
	}
    }

private:
    std::string str_;

    std::string name_;
    std::string data_;

public:
    UnprocessedEntry(const std::string& str): str_(str) {
	name_ = std::string(str);
	data_ = SplitByFirst_(name_, '\t');
    };

    const bool DataEditable = false;
    const bool NameEditable = false;
    const bool Executable = false;

    std::string GetData() const override { return data_; };
    bool SetData(std::string data) override { return false; };

    std::string GetName() const override { return name_; }
    bool SetName(std::string name) override { return false; };

    void Execute() override { return; };
    std::string GetDisplayString() override { return "..."; };
};
