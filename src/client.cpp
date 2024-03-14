#include "core.hpp"

int main(int arc, char* argv[]) {
    ServerInfo server (INADDR_ANY);
    connect_to_server(server);

    char buffer[BUFFER_SIZE];
    while (true) {
        // send request to the server
        memset(buffer, 0, sizeof(buffer));
        std::cout << "server shell $ ";
        std::cin.getline(buffer, sizeof(buffer));

        ssize_t sent = send(server.fd, buffer, sizeof(buffer), 0);
        if (sent < 0) {
            perror("send request error");
            break;
        }

        // get response from the server
        memset(buffer, 0, sizeof(buffer));
        ssize_t recieved = recv(server.fd, buffer, sizeof(buffer), 0);
        if (recieved < 0) {
            perror("get response error");
            break;
        }

        std::cout << buffer << "\n";
    }

    return 0;
}
