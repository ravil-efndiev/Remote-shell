#pragma once
#include "core.hpp"

enum class Signal {
    DEFAULT = 0,
    SHUTDOWN = 1,
};

static const std::vector<std::string> forbidden_commands {
    "vim", "nano", "nc"
};

void start_server_shell(int client_fd);
