#pragma once

#include <string>
#include <filesystem>

// Deals with parameter
class ParameterProcessor {
public:
    ParameterProcessor(const int& argc, char* argv[]): argc_(argc), argv_(argv) {}

    bool IsNumParametersValid() const { return argc_ == 3; }

    std::string GetOptionString() const { return argv_[1]; }
    std::filesystem::path GetConfigDirectory() const { return argv_[2]; }

private:
    const int& argc_;
    char** argv_;
};
