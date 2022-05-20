#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

volatile sig_atomic_t sigint_triggered=0;
void sigint_handler(int sig){
    if(sig==SIGINT){
        sigint_triggered=1;
        printf("\nReceived SIGINT signal\nexit\n");
    }
}

#define BUFFER_SIZE 500
#define SERVER_PORT 5555
#define MAX_CLIENTS 5

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
  my_address.sin_port = htons(SERVER_PORT);
  inet_pton(AF_INET, "0.0.0.0", &(my_address.sin_addr));
  memset(&(my_address.sin_zero), 0, sizeof(my_address.sin_zero));
  
  if((srv_socket = socket(AF_INET, SOCK_STREAM, 0))==-1){
    return -1;//safe exit
  }
  int yes =1;
  setsockopt(srv_socket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
  if(bind(srv_socket, (struct sockaddr *) &my_address,sizeof(struct sockaddr_in))==-1){
    perror("bind error");
    sigint_triggered=1;
  }else{
    if(listen(srv_socket,0)==-1){
      perror("listen error");
      sigint_triggered=1;
    }
  }

//read init
  int read_sockets[MAX_CLIENTS];
  for (int i = 0; i < MAX_CLIENTS; i++)
    read_sockets[i]=-1;
  
  while(sigint_triggered==0){
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(srv_socket,&read_fd);
    struct timeval select_timeout={
      .tv_sec=1,
      .tv_usec=0,
    };

    int max_sockfd=srv_socket;
    for (int i = 0; i < MAX_CLIENTS; i++){
      if(read_sockets[i]>max_sockfd)
        max_sockfd=read_sockets[i];
      if(read_sockets[i]!=-1)
        FD_SET(read_sockets[i],&read_fd);
    }

    int select_return = select(max_sockfd+1,&read_fd,NULL,NULL, &select_timeout);
    if(select_return==-1)
      continue;

    if(FD_ISSET(srv_socket,&read_fd)){
      struct sockaddr_in their_addr;
      socklen_t their_size;
      int sock_from_client = accept(srv_socket,(struct sockaddr *)&their_addr,&their_size);
      if(sock_from_client==-1){
        perror("accept error");
        continue;
      }
      char remote_address[INET6_ADDRSTRLEN];
      inet_ntop(AF_INET,&(their_addr.sin_addr),remote_address,INET_ADDRSTRLEN);
      printf("**Received connection request from %s **\n",remote_address);
      int i;
      for ( i = 0; i < MAX_CLIENTS; i++){
        if(read_sockets[i]==-1){
          read_sockets[i]=sock_from_client;
          break;
        }
      }
      if(i==MAX_CLIENTS){//reached max clients
        send(sock_from_client,NULL,0,0);
        close(sock_from_client);
      }
    } 

    for (int i = 0; i < MAX_CLIENTS; i++){
      if(read_sockets[i]!=-1 && FD_ISSET(read_sockets[i],&read_fd)){
        size_t read_bytes = recv(read_sockets[i],buffer,BUFFER_SIZE,0);
        if(read_bytes<=0){
          printf("Client n°%i is no longer online\n",i+1);
          close(read_sockets[i]);
          read_sockets[i]=-1;
        }else{
          buffer[read_bytes]='\0';
          printf("[#%i]Received : %s",i+1,buffer);
        }
      }
    }
  }
  for (int i = 0; i < MAX_CLIENTS; i++){
    if(read_sockets[i]!=1){
      send(read_sockets[i],NULL,0,0);
      close(read_sockets[i]);
    }
  }
  //CleanUp
  if(srv_socket!=-1)
    close(srv_socket);

  return 0;
}
