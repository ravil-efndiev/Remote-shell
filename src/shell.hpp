#pragma once
#include "core.hpp"

enum class Signal {
    DEFAULT = 0,
    SHUTDOWN,
    DISCONNECT,
};

static const std::vector<std::string> forbidden_commands {
    "vim", "nano", "exit", "man"
};

void start_server_shell(int client_fd);
