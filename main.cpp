#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

int server_socket = 0;                      // global for CTRL+C handler

int data_exchange();                        // get/send data
void close_handler(const int signal);       // CTRL+C signal descriptor

// [1] - port
int main(int argc, char **argv) {
    signal(SIGINT, close_handler);
    int max_connection = 10;
    int timeout_in_secosnds = 60;
    struct sockaddr_in server_address;
    if (argc != 2) {
        perror("No arguments please enter socket argument");
        return 1;
    }
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation error");
        return 2;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), 
            sizeof(server_address)) < 0) {
        perror("Socket binding error");
        return 3;
    }
    listen(server_socket, max_connection);
    // timeout {timeout_in_secosnds} sec
    struct timeval tv {static_cast<time_t>(timeout_in_secosnds), 0};
    setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, 
                reinterpret_cast<const char*>(&tv), sizeof tv);
    return data_exchange();
}

int data_exchange() {
    int current_connection;
    int bytes_read;
    const int SIZE = 1024;
    char buf[SIZE];
    struct sockaddr_in sender_address;
    socklen_t sender_len;
    char answer[] = "TCP SERVER ANSWER";
    std::cout << "Server started" << std::endl;
    while (1) {
        memset(buf, 0, sizeof buf);
        current_connection = accept(server_socket,
                                reinterpret_cast<sockaddr*>(&sender_address), 
                                &sender_len);
        if (current_connection == -1)
            continue;
        std::cout << "Connection!" << std::endl;
        // waiting for data transfer {timeout_in_seconds} seconds
        bytes_read = recv(current_connection, buf, sizeof(buf) - 1, 0);
        if (bytes_read == -1) {
            perror("Error getting data");
        }
        buf[bytes_read] = '\0'; // set \0 after message
        std::cout << "Client data: " << buf << std::endl;
        std::cout << "Client IP: " << inet_ntoa(sender_address.sin_addr) 
                    << std::endl;
        std::cout << "Client port: " 
                    << static_cast<int>(ntohs(sender_address.sin_port)) 
                    << std::endl;
        // server response
        snprintf(buf, SIZE, "SERVER: %s", answer);
        write(current_connection, buf, strlen(buf));
        close(current_connection);
    }
    close(server_socket);
    return 0;
}

void close_handler(int signal) {
    // free socket
    close(server_socket);
    exit(0);
}
