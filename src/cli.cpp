#include "cli.hpp"

Signal send_command(int server_fd) {
    std::string command_buffer;
    std::cout << COL_GREEN << "server shell $ " << COL_DEFAULT;
    std::getline(std::cin, command_buffer);

    if (std::all_of(command_buffer.begin(), command_buffer.end(), isspace)) {
        return Signal::SKIP;
    }
    else if (command_buffer == "auth") {
        std::string password;

        std::cout << "enter admin password: ";
        set_stdin_echo(false);
        std::getline(std::cin, password);
        set_stdin_echo(true);

        password = "auth:" + password;
        ssize_t sent = send(server_fd, password.data(), password.size(), 0);
        if (sent < 0) {
            perror("send request error");
            return Signal::DISCONNECT;
        }
        std::cout << "\n";
    }
    else if (command_buffer == "clear") {
        system("clear");
        return Signal::SKIP;
    }
    else {
        ssize_t sent = send(server_fd, command_buffer.data(), command_buffer.size(), 0);
        if (sent < 0) {
            perror("send request error");
            return Signal::DISCONNECT;
        }
    }
    return Signal::DEFAULT;
}

Signal get_response(int server_fd) {
    size_t data_size;
        
    ssize_t size_recieved = recv(server_fd, &data_size, sizeof(data_size), 0);
    if (size_recieved < 0) {
        perror("recieve error");
        return Signal::DISCONNECT;
    }

    std::string in_buffer (data_size, '0');
    ssize_t data_recieved = recv(server_fd, in_buffer.data(), data_size, 0); 
    if (data_recieved < 0) {
        perror("recieve error");
        return Signal::DISCONNECT;
    }
    if (in_buffer == DISCONNECT_MSG) {
        std::cout << "disconnecting from the server\n";
        return Signal::DISCONNECT;
    }
    else {
        if (in_buffer[0] == 'E') {
            std::cout << COL_RED << "Error" << COL_DEFAULT << ": "; 
            std::cout << in_buffer.substr(1) << "\n";
        }
        else {
            std::cout << in_buffer << "\n";
        }
    }
    return Signal::DEFAULT;
}

void start_client_cli(int server_fd)
{
    while (true) {
        Signal scs = send_command(server_fd);
        if (scs == Signal::DISCONNECT) break;
        else if (scs == Signal::SKIP) continue;

        Signal grs = get_response(server_fd);
        if (grs == Signal::DISCONNECT) break;
    }       
}
