#pragma once
#include <string>
#include "settings.hpp"
#include <boost/algorithm/string.hpp>

class OptionController;
class Option {
public:
    // Returns false if data delimiter exists in name
    virtual bool SetName(std::string name) {
	if (name.find_last_of(Settings::Delimiter.Data) == std::string::npos) {
	    return false;
	} else {
	    name_ = name;
	    return true;
	}
    }

    virtual bool SetData(std::string data) { 
	data_ = data;
	return true;
    }

    virtual std::string GetName() { return name_; };
    virtual std::string GetData() { return data_; }

    virtual std::string GetSaveString() { return name_ + Settings::Delimiter.Data + data_; }

    /* These parameters don't have to be used
     * pointer_to_self allows for function to replace itself
     * option_controller allows for function to operate on menu
     * If it is a nullptr, it indicates it is executing in a no menu environment
     */

    virtual int Execute(Option* pointer_to_self, const OptionController* option_controller) = 0;
protected:
    std::string name_;
    std::string data_;
};

#include <filesystem>
class DefaultOption: public Option {
public:
    DefaultOption(std::filesystem::path script_path)
	: script_path_(script_path) {}

    int Execute(Option* pointer_to_self, const OptionController* option_controller) override {
	std::string command = script_path_.string() + " " + data_;
	return WEXITSTATUS(std::system(command.c_str()));
    }

protected:
    std::filesystem::path script_path_;
};

class MenuOption: public Option {
};

class GeneratedOption: public Option {
protected:
    Option* parent_option_;

public:
    GeneratedOption(Option* parent_option)
	: parent_option_(parent_option)
    {
	name_ = parent_option_->GetName();
	data_ = parent_option_->GetData();
    }
};
