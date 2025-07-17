#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>


int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    int portNumber = 3000;
    if (client_fd < 0) {
        std::cerr << "Could not create socket. Please try again.";
        exit(0);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET; // Will connect to server via IPv4 domain
    server_address.sin_port = htons(portNumber);

    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); // Set server IP

    int server_connection = connect(client_fd, (sockaddr*)&server_address, sizeof(server_address));

    if (server_connection < 0) {
        std::cerr << "Could not connect to server" << std::endl;
        exit(0);
    }
    std::cout << "Connection successful" << std::endl;

    send(client_fd, "Hello", strlen("Hello"), 0);

    return 0;



    
}