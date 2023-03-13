#pragma once

struct ExitCode {
    static constexpr int DontExit = 99;
    static constexpr int Success = 0;
    static constexpr int LogicError = 9;
    static constexpr int ConfigLoadError = 1;
    static constexpr int WriteError = 2;
    static constexpr int UserError = 3;
};
