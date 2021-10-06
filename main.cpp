#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <iostream>

int server_socket = 0;

int data_exchange();
void close_handler(int signal);

int main(int argc, char **argv) {
    signal(SIGINT, close_handler);
    int max_connection = 10;
    if (argc != 2) {
        std::cerr << "No arguments please enter socket argument";
        return -1;
    }
    struct sockaddr_in server_address;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -2;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1])); //argv[1]
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        perror("Socket binding error");
        exit(2);
    }
    listen(server_socket, max_connection);
    return data_exchange();
}

int data_exchange() {
    int current_connection = 0;
    int bytes_read;
    const int SIZE = 1024;
    char buf[SIZE];
    struct sockaddr_in sender_address;
    socklen_t sender_len;
    std::string answer = "TCP SERVER ANSWER";

    fd_set fdset;
    struct timeval tv {10, 0}; // timeout 10 sec
    std::cout << "Server started" << std::endl;
    while (1) {
        current_connection = accept(server_socket, reinterpret_cast<sockaddr*>(&sender_address), &sender_len);
        if (current_connection == -1)
            continue;
        std::cout << "Connection!" << std::endl;
        setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

        bytes_read = recv(current_connection, buf, sizeof(buf) - 1, 0); //recv(current_connection, buf, sizeof(buf) - 1, 0); //read(current_connection, buf, sizeof(buf) - 1)
        if (bytes_read == -1) {
            perror("Error getting data");
        }
        buf[bytes_read] = '\0';
        std::cout << "Client data: " << buf << std::endl;
        std::cout << "Client IP: " << inet_ntoa(sender_address.sin_addr) << std::endl;
        std::cout << "Client port: " << static_cast<int>(ntohs(sender_address.sin_port)) << std::endl;

        snprintf(buf, SIZE, "SERVER: %s", answer.c_str());
        write(current_connection, buf, strlen(buf));
        close(current_connection);
    }
    close(server_socket);
    return 0;
}

void close_handler(int signal) {
    // free socket
    close(server_socket);
    // exit
    exit(0);
}
