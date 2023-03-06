#pragma once

#include <string>
#include <filesystem>
#include <locale>
#include <codecvt>

// Deals with parameter
class ParameterProcessor {
public:
    ParameterProcessor(const int& argc, char* argv[]): argc_(argc), argv_(argv) {}

    bool IsNumParametersValid() const { return argc_ == 2; }

    std::wstring GetOptionString() const {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(argv_[1]);
    }

private:
    const int& argc_;
    char** argv_;
};
