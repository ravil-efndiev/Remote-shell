#include "cli.hpp"

int main(int arc, char* argv[]) {
    ServerInfo server (INADDR_ANY);
    connect_to_server(server);
    
    print_client_wellcome();
    start_client_cli(server.fd);    
    
    close(server.fd);
    return 0;
}
