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

    try {
        std::string dump = get_text_from_file("/tmp/.cmddump");
        return dump.empty() ? "no output" : dump;
    }
    catch (std::runtime_error err) {
        std::cerr << err.what() << "\n";
        return std::string(COL_RED) + "command error" + COL_DEFAULT;
    }
}

Signal run_shell(std::string& request, int client_fd, FILE* shell) {
    auto colon_tokens = split_str(request, ':');
    std::string command_name = split_str(request, ' ')[0];
    std::string password;
    try {
        std::string env = get_text_from_file(".env");
        auto envlines = split_str(env, '\n');
        for (const auto& line : envlines) {
            auto vars = split_str(line, '=');
            if (vars[0] == "PASSWORD") {
                password = vars[1];
                break;
            }
        }
    } 
    catch (std::runtime_error err) {
        std::cerr << err.what() << "\n";
        exit(1);
    }

    if (colon_tokens[0] == "auth" && colon_tokens.size() == 2) {
        if (colon_tokens[1] == password)
        {
            send_response(client_fd, "Access to shell allowed");
            verified = true;
        } 
        else {
            send_response(client_fd, std::string(COL_RED) + "credentianls incorrect"  + COL_DEFAULT);
        }
    }
    else if (std::find(forbidden_commands.begin(), forbidden_commands.end(), command_name) != forbidden_commands.end()) {
        send_response(client_fd, std::string(COL_RED) + "This command is not allowed" + COL_DEFAULT);
        return Signal::DEFAULT;
    }
    else if (request == "disconnect") {
        send_response(client_fd, "disconnecting from the server");
        close(client_fd);
        return Signal::DISCONNECT;
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
            send_response(client_fd, std::string(COL_RED) + "you are not authenticated, use `auth` to get access" + COL_DEFAULT);
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
        if (result == Signal::SHUTDOWN || result == Signal::DISCONNECT)
            break;
    }

    remove("/tmp/.cmddump");
}
 