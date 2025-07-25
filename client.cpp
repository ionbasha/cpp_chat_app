#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>


void handle_server_read(int client_fd) {
    char buffer[1024] = {0};

    while(1) {
        int num_bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
        if (num_bytes_read > 0) {
            std::cout << std::string(buffer, num_bytes_read) << std::endl;
        }
        else if (num_bytes_read == 0) {
            std::cout << "Client " << client_fd << " disconnected" << std::endl;
            break;
        }
        else {
            std::cerr << "Error reading from client, exiting..." << std::endl;
            break;
        }
    }
    close(client_fd);
}


int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    int PORT = 3000;

    if (client_fd < 0) {
        std::cerr << "Could not create socket. Please try again.";
        exit(0);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET; // Connect to server via IPv4 domain
    server_address.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); // Convert IP to binary
 
    int server_connection = connect(client_fd, (sockaddr*)&server_address, sizeof(server_address));

    if (server_connection < 0) {
        std::cerr << "Could not connect to server" << std::endl;
        exit(0);
    }
    std::cout << "Connection successful." << std::endl;

    std::thread server_read_thread([client_fd]() { // Handle client messages in a background thread
        handle_server_read(client_fd);
    });
    server_read_thread.detach();

    std::string input;
    while(1) {
        std::getline(std::cin, input);
        if (input == "quit()") {
            break;
        }
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