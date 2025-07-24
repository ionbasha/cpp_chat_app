#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <thread>
#include <vector>

void handle_client(int client_socket, std::vector<int>& client_fds) {
    char buffer[1024] = {0};

    while(1) {
        int num_bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (num_bytes_read > 0) {
            std::cout << "Client " << client_socket << ": " << std::string(buffer, num_bytes_read) << std::endl;
        }
        else if (num_bytes_read == 0) {
            client_fds.erase(std::remove(client_fds.begin(), client_fds.end(), client_socket), client_fds.end());
            std::cout << "Client" << client_socket << " disconnected" << std::endl;
            break;
        }
        else {
            std::cerr << "Error reading from client, exiting..." << std::endl;
            break;
        }
        for(int i : client_fds) { // TESTING: REMOVE LATER
            std::cout << i << std::endl;
        }
    }
    close(client_socket);
}

int main() {
    // AF_NET: specificies IPv4 address family used by server socket
    // SOCK_STREAM: socket uses TCP connection
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    short PORT = 3000;

    std::vector<int> client_fd_list; // Vector containing list of clients connected to server

    if (server_fd < 0) {
        std::cerr << "Could not create socket. Please try again.";
        exit(0);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    int server_start = bind(server_fd, (sockaddr*)&server_address, sizeof(server_address));

    if (server_start < 0) {
        std::cerr << "Could not start server" << std::endl;
        exit(0);
    }

    std::cout << "Server started" << std::endl;

    listen(server_fd, SOMAXCONN);
    std::cout << "Server listening on port " << PORT << std::endl;

    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);

    while(1) {
        int client_connection_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_address_size);

        if (client_connection_socket < 0) {
            std::cerr << "A client connection failed" << std::endl;
            exit(0);
        }

        client_fd_list.push_back(client_connection_socket);

        std::thread client_message_thread([client_connection_socket, &client_fd_list]() { // Handle client messages in a background thread
            handle_client(client_connection_socket, client_fd_list);
        });
        client_message_thread.detach();
    }

    close(server_fd);
    return 0;
}