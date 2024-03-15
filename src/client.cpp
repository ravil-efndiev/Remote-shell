#include "core.hpp"

int main(int arc, char* argv[]) {
    ServerInfo server (INADDR_ANY);
    connect_to_server(server);
    print_client_wellcome();
                       
    char* in_buffer;
    std::string command_buffer;
    while (true) {
        // send request to the server
        std::cout << COL_GREEN << "server shell $ " << COL_DEFAULT;
        std::getline(std::cin, command_buffer);

        if (command_buffer == "auth") {
            std::string password;

            std::cout << "enter admin password: ";
            set_stdin_echo(false);
            std::getline(std::cin, password);
            set_stdin_echo(true);

            password = "auth:" + password;
            ssize_t sent = send(server.fd, password.data(), password.size(), 0);
            if (sent < 0) {
                perror("send request error");
                break;
            }
            std::cout << "\n";
        }
        else {
            ssize_t sent = send(server.fd, command_buffer.data(), command_buffer.size(), 0);
            if (sent < 0) {
                perror("send request error");
                break;
            }
        }

        // get response from the server
        size_t data_size;
        
        ssize_t size_recieved = recv(server.fd, &data_size, sizeof(data_size), 0);
        if (size_recieved < 0) {
            perror("recieve error");
            break;
        }

        in_buffer = new char[data_size];
        ssize_t data_recieved = recv(server.fd, in_buffer, data_size, 0); 
        if (data_recieved < 0) {
            perror("recieve error");
            break;
        }
        std::cout << in_buffer << "\n";
        delete[] in_buffer;
    }

    return 0;
}
