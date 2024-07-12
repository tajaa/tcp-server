#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);

  // Create socket file descriptor
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    fprintf(stderr, "Socket creation error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    fprintf(stderr, "Setsockopt error: %s\n", strerror(errno));
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Setup server address
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket to specified address
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    fprintf(stderr, "Bind failed: %s\n", strerror(errno));
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Start listening for incoming connections
  if (listen(server_fd, 3) < 0) {
    fprintf(stderr, "Listen failed: %s\n", strerror(errno));
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  // Accept an incoming connection
  new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
  if (new_socket < 0) {
    fprintf(stderr, "Accept failed: %s\n", strerror(errno));
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Connection accepted\n");

  // Close sockets
  close(new_socket);
  close(server_fd);

  return 0;
}
