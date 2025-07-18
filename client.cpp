#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>


int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    int portNumber = 3000;
    if (client_fd < 0) {
        std::cerr << "Could not create socket. Please try again.";
        exit(0);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET; // Connect to server via IPv4 domain
    server_address.sin_port = htons(portNumber);

    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); // Convert IP to binary
 
    int server_connection = connect(client_fd, (sockaddr*)&server_address, sizeof(server_address));

    if (server_connection < 0) {
        std::cerr << "Could not connect to server" << std::endl;
        exit(0);
    }
    std::cout << "Connection successful." << std::endl;

    std::string input;

    while(1) {
        std::getline(std::cin, input);
        if (!input.empty()) {
            int bytes_sent = send(client_fd, input.c_str(), input.size(), 0);
            if(bytes_sent < 0) {
                std::cerr << "Could not send request to server" << std::endl;
                break;
            }
        }
    }

    close(client_fd);
    return 0;



    
}