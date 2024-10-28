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
  const char *message = "Hello from client!";

  // Struct for the server information
  struct sockaddr_in servaddr;

  // Creating UDP socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation FAILED!");
    exit(EXIT_FAILURE);
  }

  // Initializing struct with zeroes
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERVER_PORT);
  // Accept connection on any available IP
  servaddr.sin_addr.s_addr = INADDR_ANY;

  // Number of recieved amount of bytes
  int numBytes;
  // Length of recieved sockaddr_in struct
  socklen_t length;

  // Send the message
  sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  std::cout << "Message to the server sent." << std::endl;

  // Recieving message from the server
  numBytes = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                      (struct sockaddr *)&servaddr, &length);
  // Add end of the string
  buffer[numBytes] = '\0';

  std::cout << "Server: " << buffer << std::endl;

  close(sockfd);

  return 0;
}