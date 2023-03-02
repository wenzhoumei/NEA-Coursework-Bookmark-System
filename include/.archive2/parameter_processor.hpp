#pragma once

#include <string>
#include <filesystem>

// Deals with parameter
class ParameterProcessor {
public:
    ParameterProcessor(const int& argc, char* argv[]): argc_(argc), argv_(argv) {}

    bool IsNumParametersValid() const { return argc_ == 2; }

    std::string GetOptionString() const { return argv_[1]; }

private:
    const int& argc_;
    char** argv_;
};
