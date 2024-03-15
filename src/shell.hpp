#pragma once
#include "core.hpp"

static const std::vector<std::string> forbidden_commands {
    "vim", "nano", "exit", "man"
};

void start_server_shell(int client_fd);
