#include <fcntl.h>
#include "shell.hpp"

namespace {
    bool verified;
}

void send_response(int client_fd, const std::string& data) {
    ssize_t sent = send(client_fd, data.data(), data.size(), 0);

    if (sent < 0) { 
        perror("server write error");
        exit(1);
    }
}

std::string run_command(const std::string& command, FILE* shell) {
    fprintf(shell, "%s > /tmp/.cmddump\n", command.c_str());
    fflush(shell);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::ifstream dump ("/tmp/.cmddump", std::ios::in);

    if (!dump) {
        std::cerr << "dump file cannot be opened\n";
        return "command error"; 
    } 

    std::stringstream ss;
    ss << dump.rdbuf();
    std::string result = ss.str();

    return result.empty() ? "no output" : result;
}

Signal run_shell(std::string& request, int client_fd, FILE* shell) {
    auto tokens = split_str(request, ':');

    if (tokens[0] == "auth" && tokens.size() == 2) {
        if (tokens[1] == "password123")
        {
            send_response(client_fd, "Access to shell allowed");
            verified = true;
        } 
        else {
            send_response(client_fd, "credentianls incorrect");
        }
    }
    else if (request == "disconnect")
    {
        send_response(client_fd, "disconnecting from the server");
        close(client_fd);
    }
    else {
        if (verified) {
            if (request == "shutdown") {
                send_response(client_fd, "shutting down the server");
                return Signal::SHUTDOWN;
            }
            else {
                std::string result = run_command(request, shell);
                send_response(client_fd, result);
            }
        }
        else {
            send_response(client_fd, "you are not authenticated, use auth:`password` to authenticate");
        }
    }
    return Signal::DEFAULT;
}

void start_server_shell(int client_fd) {
    std::unique_ptr<FILE, decltype(&pclose)> shell {popen("/bin/bash", "w"), pclose};
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        ssize_t recieved = recv(client_fd, buffer, sizeof(buffer), 0);
        if (recieved < 0) { 
            perror("server read error");
            continue;
        }

        std::string request (buffer);
        std::cout << "request to server: " << request << '\n';

        Signal result = run_shell(request, client_fd, shell.get());        
        if (result == Signal::SHUTDOWN)
            break;
    }

    remove("/tmp/.cmddump");
}
