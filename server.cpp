#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 512

int main() {
  // Socket file descriptor
  int sockfd;

  // Buffer for the messages
  char buffer[BUFFER_SIZE];

  // Message that'll be sending
  const char *message = "Hello from server";

  // Struct for the server and client information
  struct sockaddr_in servaddr, cliaddr;

  // Creating UDP socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation FAILED!");
    exit(EXIT_FAILURE);
  }

  // Initialising structs for server and client
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(SERVER_PORT);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Bind FAILED");
    exit(EXIT_FAILURE);
  }

  // Number of recieved amount of bytes
  int numBytes;
  // Length of recieved sockaddr_in struct
  socklen_t length;

  length = sizeof(cliaddr);

  // Recieving message from the client
  numBytes = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                      (struct sockaddr *)&cliaddr, &length);
  // Add end of the string
  buffer[numBytes] = '\0';
  printf("Client: %s\n", buffer);
  // Send respond to the client
  sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM,
         (const struct sockaddr *)&cliaddr, length);

  std::cout << "Message sent." << std::endl;

  return 0;
}