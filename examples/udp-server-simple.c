#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t sigint_triggered=0;
void sigint_handler(int sig){
    if(sig==SIGINT){
        sigint_triggered=1;
        printf("\nReceived SIGINT signal\nexit\n");
    }
}
#define BUFFER_SIZE 500

int main(int argc, char *argv[]) {

  struct sigaction my_sigint_action={
    .sa_handler= sigint_handler,
    .sa_mask = SIGINT,
    .sa_flags=SA_RESETHAND,
    .sa_restorer=NULL,
  };
  sigaction(SIGINT,&my_sigint_action,NULL);
  
  char buffer[BUFFER_SIZE+1];
  struct sockaddr_in my_address;
  int srv_socket;

  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(5555);
  inet_pton(AF_INET, "0.0.0.0", &(my_address.sin_addr));
  memset(&(my_address.sin_zero), 0, sizeof(my_address.sin_zero));
  
   if((srv_socket=socket(AF_INET, SOCK_DGRAM,0))==-1){
        return -1; //safe exit
    }
  
  if(bind(srv_socket, (struct sockaddr *) &my_address,sizeof(struct sockaddr_in))==-1){
    sigint_triggered=1;
  }

  while(sigint_triggered==0) {
    struct sockaddr_in their_addr;
    socklen_t their_size;
    size_t bytes_read = recvfrom(srv_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&their_addr, &their_size) ;
    if(bytes_read<0 && errno==EINTR)
      break;

    buffer[bytes_read]='\0';
    printf("Received : %s\nfrom: %s\n",buffer,inet_ntoa(their_addr.sin_addr));
  }
  // Cleanup
   if(srv_socket!=-1){
        close(srv_socket);
    }
  return 0;
}
