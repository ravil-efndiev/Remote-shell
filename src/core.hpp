#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

const uint16_t PORT = 8080;
const uint16_t BUFFER_SIZE = 1024;

struct ServerInfo {
    int fd = -1;
    struct sockaddr_in address;
    size_t addr_size;

    ServerInfo(in_addr_t addr);
};

void init_server(const ServerInfo& server);

void connect_to_server(const ServerInfo& server);

std::vector<std::string> split_str(const std::string& str, char separator);

void set_stdin_echo(bool enable);
