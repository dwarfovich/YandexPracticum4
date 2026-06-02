#include "file.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#endif

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

namespace analyzer::file {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

File::File(const std::string &filename) : name{filename} {
    std::ifstream file(name);

    if (!file.is_open()) {
        throw std::invalid_argument("Can't open file " + filename);
    }
    ast = GetAst(filename);
    source_lines = ReadSourceFile(file);
}

std::vector<std::string> File::ReadSourceFile(std::ifstream &file) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::string File::GetAst(const std::string &filename) try {
    std::string full_cmd = File::command_prefix + filename + " 2>&1";

    std::string result;
    std::array<char, 256> buffer;

#ifdef _WIN32
    HANDLE read_pipe = nullptr;
    HANDLE write_pipe = nullptr;

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&read_pipe, &write_pipe, &sa, 0)) {
        throw std::runtime_error("CreatePipe failed");
    }

    SetHandleInformation(read_pipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = write_pipe;
    si.hStdError = write_pipe;

    PROCESS_INFORMATION pi{};

    if (!CreateProcess(nullptr, full_cmd.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) {
        CloseHandle(read_pipe);
        CloseHandle(write_pipe);

        throw std::runtime_error("CreateProcess failed, error code: " + std::to_string(GetLastError()));
    }

    CloseHandle(write_pipe);

    DWORD bytes_read = 0;
    while (ReadFile(read_pipe, buffer.data(), static_cast<DWORD>(buffer.size()), &bytes_read, nullptr) &&
           bytes_read > 0) {
        result.append(buffer.data(), bytes_read);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code = 0;
    GetExitCodeProcess(pi.hProcess, &exit_code);

    CloseHandle(read_pipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exit_code != 0) {
        throw std::runtime_error("Process failed with code " + std::to_string(exit_code));
    }
#else
    using PipePtr = std::unique_ptr<FILE, decltype([](FILE *pipe) {
                                        if (!pipe)
                                            return;

                                        int status = pclose(pipe);
                                        if (WIFEXITED(status)) {
                                            int exit_status = WEXITSTATUS(status);
                                            if (exit_status != 0) {
                                                throw std::runtime_error("Command failed with exit code " +
                                                                         std::to_string(exit_status));
                                            }
                                        } else {
                                            throw std::runtime_error("Command terminated abnormally");
                                        }
                                    })>;

    FILE *raw_pipe = popen(full_cmd.c_str(), "r");
    if (!raw_pipe) {
        throw std::runtime_error("Failed to execute command: " + std::string(std::strerror(errno)));
    }
    PipePtr pipe(raw_pipe);

    while (fgets(buffer.data(), buffer.size(), pipe.get())) {
        result += buffer.data();
    }
#endif
    return result;
} catch (const std::exception &e) {
    throw std::runtime_error("Error while getting ast from " + filename);
}

}  // namespace analyzer::file
