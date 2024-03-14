#pragma once
#include "core.hpp"

enum class Signal {
    DEFAULT = 0,
    SHUTDOWN = 1,
};

static const std::vector<std::string> forbiddenCommands {
    "vim", "nano"
};

void start_server_shell(int client_fd);
