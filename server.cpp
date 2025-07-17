#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h>


int main() {
    // AF_NET: specificies IPv4 address family used by server socket
    // SOCK_STREAM: socket uses TCP connection
    // 0 indicates bas
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    short portNumber = 3000;
    if (server_fd < 0) {
        std::cerr << "Could not create socket. Please try again.";
        exit(0);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portNumber);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    int server_start = bind(server_fd, (sockaddr*)&server_address, sizeof(server_address));

    if (server_start < 0) {
        std::cerr << "Could not start server" << std::endl;
        exit(0);
    }

    std::cout << "Server started" << std::endl;

    listen(server_fd, SOMAXCONN);
    std::cout << "Server listening on port " << portNumber << std::endl;

    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    
    int client_connection_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_address_size);

    if (client_connection_socket < 0) {
        std::cerr << "A client connection failed" << std::endl;
        exit(0);
    }

    std::string msg = "Message received!";
    send(client_connection_socket, msg.c_str(), msg.size(), 0);

    char read_buffer[1024] = {0};

    while(1) {
        int num_bytes_read = recv(client_connection_socket, read_buffer, sizeof(read_buffer), 0);
        if (num_bytes_read > 0) {
            std::cout << "Client " << client_connection_socket << ": " << std::string(read_buffer, num_bytes_read) << std::endl;
        }
        else if (num_bytes_read == 0) {
            std::cout << "Client disconnected, exiting..." << std::endl;
            break;
        }
        else {
            std::cerr << "Error reading from client, exiting..." << std::endl;
            break;
        }
    }
    


    close(server_fd);
    close(client_connection_socket);

    return 0;
}