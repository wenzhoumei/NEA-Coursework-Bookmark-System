#include "parameter_processor.hpp"

#include <locale>
#include <codecvt>

ParameterProcessor::ParameterProcessor(const int& argc, char* argv[]): argc_(argc), argv_(argv) {}

bool ParameterProcessor::IsNumParametersValid() const { return argc_ <= 2; }

std::wstring ParameterProcessor::GetOptionString() const {
    if (argc_ == 1) { return L"root@bmk"; }
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(argv_[1]);
}
