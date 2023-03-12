#pragma once

#include <string>

/**
 * @brief Used to process information of parameters
 */
class ParameterProcessor {
public:
    ParameterProcessor(const int& argc, char* argv[]);

    /**
     * @brief Does it have a valid number of parameters
     */
    bool IsNumParametersValid() const;

    /**
     * @brief Gets option string to be executed on by parser
     */
    std::wstring GetOptionString() const;

private:
    const int& argc_;
    char** argv_;
};
