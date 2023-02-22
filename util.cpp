#include "util.hpp"

std::string WStringToString(std::wstring wide_str) {

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wide_str);
}
