#include "core.hpp"

ServerInfo::ServerInfo(in_addr_t addr) {
    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        perror("socket error");
        exit(1);
    }

    addr_size = sizeof(address);

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(addr);
}

void init_server(const ServerInfo& server) {
    if (bind(server.fd, (struct sockaddr*)&server.address, server.addr_size) != 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(server.fd, SOMAXCONN) != 0) {
        perror("listen error");
        exit(1);
    }
}

void connect_to_server(const ServerInfo& server) {
    int cstatus = connect(server.fd, (struct sockaddr*)&server.address, server.addr_size);
    if (cstatus < 0) {
        perror("connect error");
        exit(1);
    }
}

std::vector<std::string> split_str(const std::string& str, char separator) {
    std::vector<std::string> tokens;

    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {
        
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            tokens.push_back(temp);
            startIndex = endIndex + 1;
        }
    }

    return tokens;
}

void set_stdin_echo(bool enable)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

std::string get_text_from_file(const std::string& path)
{
    std::ifstream file (path, std::ios::in);
    if (!file) {
        throw std::runtime_error("cannot open file `" + path + "`");
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void print_client_wellcome() {
    std::cout << R"( /$$$$$$$                                                     )" << "\n";    
    std::cout << R"(| $$__  $$                                                    )" << "\n";    
    std::cout << R"(| $$  \ $$  /$$$$$$  /$$$$$$/$$$$  /$$  /$$  /$$ /$$  /$$  /$$)" << "\n";    
    std::cout << R"(| $$$$$$$/ /$$__  $$| $$_  $$_  $$| $$ | $$ | $$| $$ | $$ | $$)" << "\n";    
    std::cout << R"(| $$__  $$| $$$$$$$$| $$ \ $$ \ $$| $$ | $$ | $$| $$ | $$ | $$)" << "\n";    
    std::cout << R"(| $$  \ $$| $$_____/| $$ | $$ | $$| $$ | $$ | $$| $$ | $$ | $$)" << "\n";    
    std::cout << R"(| $$  | $$|  $$$$$$$| $$ | $$ | $$|  $$$$$/$$$$/|  $$$$$/$$$$/)" << "\n";    
    std::cout << R"(|__/  |__/ \_______/|__/ |__/ |__/ \_____/\___/  \_____/\___/ )" << "\n";    
    std::cout << "Welocme to Remww remote shell\n";
    std::cout << "Use " << COL_BLUE << "disconnect" << COL_DEFAULT << " to disconnect from the server\n";
    std::cout << "Use " << COL_BLUE << "auth" << COL_DEFAULT << " to get access to server shell\n";
}

void gen_env_file(const std::string& password) {
    std::ofstream env (".env", std::ios::out);
    if (!env) {
        std::cerr << "cant create or open .env file\n";
        exit(1);
    }
    env << "PASSWORD=" << password << '\n';
}
