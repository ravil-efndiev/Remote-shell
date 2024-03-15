#include "shell.hpp"

int main(int argc, char* argv[]) {
    ServerInfo server (INADDR_ANY);
    init_server(server);

    int client_fd = accept(server.fd, (struct sockaddr*)&server.address, (socklen_t*)&server.addr_size);
    if (client_fd < 0) {
        perror("client connection error");
        return 1;
    }

    start_server_shell(client_fd);
    close(client_fd);
    close(server.fd);

    return 0;
}
