#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_EVENTS 10
#define MAX_REQUEST_SIZE 1024

void handle_request(int client_socket) {
    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

int main() {
    // Create kqueue
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server addresses and ports
    struct sockaddr_in server_addresses[] = {
        { .sin_family = AF_INET, .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(8080) },
        { .sin_family = AF_INET, .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(8081) }
        // Add more addresses as needed
    };

    // Create and bind sockets for each address
    int server_sockets[sizeof(server_addresses) / sizeof(server_addresses[0])];
    for (int i = 0; i < sizeof(server_addresses) / sizeof(server_addresses[0]); ++i) {
        server_sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sockets[i] == -1) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }

        if (bind(server_sockets[i], (struct sockaddr*)&server_addresses[i], sizeof(server_addresses[i])) == -1) {
            perror("Bind failed");
            close(server_sockets[i]);
            exit(EXIT_FAILURE);
        }

        if (listen(server_sockets[i], 5) == -1) {
            perror("Listen failed");
            close(server_sockets[i]);
            exit(EXIT_FAILURE);
        }

        // Set up kevent structure for server socket
        struct kevent server_event;
        EV_SET(&server_event, server_sockets[i], EVFILT_READ, EV_ADD, 0, 0, NULL);

        // Attach the server socket to the kqueue
        if (kevent(kq, &server_event, 1, NULL, 0, NULL) == -1) {
            perror("Failed to register server socket with kqueue");
            close(server_sockets[i]);
            close(kq);
            exit(EXIT_FAILURE);
        }

        printf("Listening on port %d\n", ntohs(server_addresses[i].sin_port));
    }

    printf("HTTP Server started. Waiting for connections...\n");

    while (1) {
        // Wait for events
        struct kevent events[MAX_EVENTS];
        int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (nev == -1) {
            perror("kevent failed");
            break;
        }

        // Handle events
        for (int i = 0; i < nev; ++i) {
            int fd = events[i].ident;

            if (fd == server_sockets[0] || fd == server_sockets[1]) {
                // New connection
                struct sockaddr_in client_address;
                socklen_t client_len = sizeof(client_address);
                int client_socket = accept(fd, (struct sockaddr*)&client_address, &client_len);

                if (client_socket == -1) {
                    perror("Accept failed");
                    continue;
                }

                // Set up kevent structure for client socket
                struct kevent client_event;
                EV_SET(&client_event, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);

                // Attach the client socket to the kqueue
                if (kevent(kq, &client_event, 1, NULL, 0, NULL) == -1) {
                    perror("Failed to register client socket with kqueue");
                    close(client_socket);
                } else {
                    printf("Accepted connection from %s:%d\n",
                           inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                }
            } else {
                // Data available for an existing connection
                char buffer[MAX_REQUEST_SIZE];
                ssize_t bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);

                if (bytesRead <= 0) {
                    // Connection closed or error
                    if (bytesRead == 0) {
                        printf("Connection closed by client\n");
                    } else {
                        perror("recv failed");
                    }

                    close(fd);
                } else {
                    // Null-terminate the received data
                    buffer[bytesRead] = '\0';
                    printf("Received request:\n%s\n", buffer);

                    // Handle the HTTP request and send a response
                    handle_request(fd);
                }
            }
        }
    }

    // Clean up
    for (int i = 0; i < sizeof(server_sockets) / sizeof(server_sockets[0]); ++i) {
        close(server_sockets[i]);
    }
    close(kq);

    return 0;
}
