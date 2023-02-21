#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include "util.hpp"

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

    std::wstring GetString() const {
	if (GetData() == L"") { return GetName(); }
	else { return GetName() + L'\t' + GetData(); }
    }

    const bool DataEditable = false;
    const bool NameEditable = false;
    const bool Executable = false;

    virtual std::wstring GetData() const = 0;
    virtual bool SetData(std::wstring data) = 0;

    virtual std::wstring GetName() const = 0;
    virtual bool SetName(std::wstring name) = 0;

    virtual void Execute() = 0;
    virtual std::wstring GetDisplayString() = 0;
};

class DefaultEntry: public Entry {
protected:
    std::wstring name_;
    std::wstring script_path_;
    std::wstring data_;

    void RunScript_() {
	status = RUNNING;
	std::wstring command = script_path_ + L" " + data_;

	int exit_code = WEXITSTATUS(std::system(WStringToString(command).c_str()));

	UpdateStatus(exit_code);
    }

public:
    const bool DataEditable = true;
    const bool NameEditable = true;
    const bool Executable = true;

    DefaultEntry(const std::wstring& name, const std::wstring& script_path, const std::wstring& data)
	: name_(name), script_path_(script_path), data_(data)
    {
    }

    std::wstring GetData() const override { return data_; }
    bool SetData(std::wstring data) override {
	data_ = data;
	return true;
    };

    std::wstring GetName() const override { return name_; }
    bool SetName(std::wstring name) override {
	name_ = name;
	return true;
    };

    void Execute() override {
	RunScript_();
    }

    std::wstring GetDisplayString() override {
	return name_;
    }
};

class UnprocessedEntry: public Entry {
private:
    std::wstring SplitByFirst_(std::wstring& str, char del) {
	size_t first_del_pos = str.find(del);
	if (first_del_pos == std::wstring::npos) {
	    // Delimiter not found, return empty string
	    return L"";
	} else {
	    // Split string into two parts
	    std::wstring first_part = str.substr(0, first_del_pos);
	    std::wstring end_part = str.substr(first_del_pos + 1);

	    str = first_part;
	    return end_part;
	}
    }

private:
    std::wstring str_;

    std::wstring name_;
    std::wstring data_;

public:
    UnprocessedEntry(const std::wstring& str): str_(str) {
	name_ = std::wstring(str);
	data_ = SplitByFirst_(name_, ',');
    };

    const bool DataEditable = false;
    const bool NameEditable = false;
    const bool Executable = false;

    std::wstring GetData() const override { return data_; };
    bool SetData(std::wstring data) override { return false; };

    std::wstring GetName() const override { return name_; }
    bool SetName(std::wstring name) override { return false; };

    void Execute() override { return; };
    std::wstring GetDisplayString() override { return L"..."; };
};
