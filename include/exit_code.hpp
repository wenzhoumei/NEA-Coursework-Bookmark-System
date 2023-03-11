#pragma once

struct ExitCode {
    static constexpr int DontExit = 99;
    static constexpr int Success = 0;
    static constexpr int LogicError = 9;
};
