#include "shell.hpp"

int main(int argc, char* argv[]) {
    ServerInfo server (INADDR_ANY);
    init_server(server);
    if (argc > 1) gen_env_file(argv[1]);

    std::cout << "Remww has been hosted, waiting for connections...\n";

    int client_fd = accept_connection(server);    
    std::cout << "connection with client accepted\n";

    start_server_shell(client_fd);
    close(client_fd);
    close(server.fd);

    return 0;
}
