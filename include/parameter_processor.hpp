#pragma once

#include <string>

// Deals with parameter
class ParameterProcessor {
public:
    ParameterProcessor(const int& argc, char* argv[]);

    bool IsNumParametersValid() const;
    std::wstring GetOptionString() const;

private:
    const int& argc_;
    char** argv_;
};
