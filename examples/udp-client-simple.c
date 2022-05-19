#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 500

int main(int argc, char *argv[]) {
  char buffer[BUFFER_SIZE+1];
  struct sockaddr_in remote_address;
  int sock_to_server;

  remote_address.sin_family = AF_INET;
  remote_address.sin_port = htons(5555);
  inet_pton(AF_INET, "127.0.0.1", &(remote_address.sin_addr));
  memset(&(remote_address.sin_zero), 0, sizeof(remote_address.sin_zero));
  sock_to_server = socket(AF_INET, SOCK_DGRAM, 0); // Check for errors

  while(1) {
    printf("Enter your message: ");
    fflush(stdout);
    size_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
    buffer[bytes_read] = '\0';
    size_t bytes_sent = sendto(sock_to_server, buffer, bytes_read+1, 0, \
      (struct sockaddr *) &remote_address, sizeof(struct sockaddr_in));
  }
  // TODO: Cleanup
  return 0;
}
