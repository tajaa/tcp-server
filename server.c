#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// #define directive

#define PORT 8080
#define BUFFER_SIZE 1024

// entry point

int main() {
  int server_fd, new_socket;
  // struct is like a class--struct holds address info
  struct sockaddr_in address;
  // int var for socket options
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};

  // create socketifle discriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  // set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // setup server address
  // set the address family to AF_INET (IPv4)
  address.sin_family = AF_INET;
  // accepting connections on any available interface/comm mean
  address.sin_addr.s_addr = INADDR_ANY;
  // set port number in network byte order
  address.sin_port = htons(PORT);

  // bind socket to spec address
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind fails");
    exit(EXIT_FAILURE);
  }

  // start listening for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("listening failed");
    exit(EXIT_FAILURE);
  }
  // print a message indicating the server is listening to specified port
  printf("Server listening on port %d\n", PORT);

  // acept incoming connections

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("connection failed");
    exit(EXIT_FAILURE);
  }
  printf("connection accepted\n");

  // read data from client and print it
  // declare a signed size type variable for the number of bytes read

  ssize_t valread;
  while ((valread = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
    printf("Client: %s", buffer);
    memset(buffer, 0, sizeof(buffer));
  }
  // close socket
  close(server_fd);
  return 0;
}
