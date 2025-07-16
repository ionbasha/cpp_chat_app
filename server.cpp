#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> 


int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        std::cerr << "Could not create socket. Please try again.";
        exit(0);
    }
    

    
}