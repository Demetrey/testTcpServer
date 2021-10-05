#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>

int data_exchange(const int sock);

int main(int argc, char **argv) {
    int max_connection = 10;
    if (argc != 2) {
        std::cerr << "No arguments please enter socket argument";
        return -1;
    }
    struct sockaddr_in server_address;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -2;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        perror("Socket binding error");
        exit(2);
    }
    listen(server_socket, max_connection);
    return data_exchange(server_socket);
}

int data_exchange(const int server_socket) {
    int current_connection = 0;
    int bytes_read;
    const int SIZE = 1024;
    char buf[SIZE];
    struct sockaddr_in sender_address;
    socklen_t sender_len;
    std::string answer = "TCP SERVER ANSWER";
    while (1) {
        current_connection = accept(server_socket, reinterpret_cast<sockaddr*>(&sender_address), &sender_len);
        std::cout << "Connection!" << std::endl;
        //bytes_read = (recvfrom(server_socket, buf, SIZE, 0, reinterpret_cast<sockaddr*>(&sender_address), &sender_len));
        bytes_read = read(current_connection, buf, sizeof(buf) - 1);
        if (bytes_read == -1) {
            perror("Error getting data");
            return -3;
        }
        buf[bytes_read] = '\0';
        std::cout << "Client data: " << buf << std::endl;
        std::cout << "Client IP: " << inet_ntoa(sender_address.sin_addr) << std::endl;
        std::cout << "Client port: " << static_cast<int>(ntohs(sender_address.sin_port)) << std::endl;

        snprintf(buf, SIZE, "SERVER: %s", answer.c_str());
        write(current_connection, buf, strlen(buf));
        close(current_connection);
    }
    return 0;
}
