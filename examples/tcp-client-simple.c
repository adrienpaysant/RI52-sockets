#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

volatile sig_atomic_t sigint_triggered=0;
void sigint_handler(int sig){
    if(sig==SIGINT){
        sigint_triggered=1;
        printf("\nReceived SIGINT signal\nexit\n");
    }
}

#define BUFFER_SIZE 500

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);

    char buffer[BUFFER_SIZE+1];
    struct sockaddr_in remote_address;
    int sock_to_server;

    //parameters for port & address
    int port=0;
    char addr[15];

    for(int i=0;i<argc;i++){
        if(argv[i][0]=='-'){
            if(argv[i][1]=='h'){
                strcpy(addr,argv[i+1]);
            }else if (argv[i][1]=='p'){
                port=atoi(argv[i+1]);
            }
        }
    }

    remote_address.sin_family = AF_INET;

    if(port==0){
        remote_address.sin_port = htons(5555);
    }else{
        remote_address.sin_port = htons(port);
    }
    if(!strcmp(addr,"")){
        inet_pton(AF_INET, "127.0.0.1", &(remote_address.sin_addr));
    }else{
        inet_pton(AF_INET, addr, &(remote_address.sin_addr));
      }

    memset(&(remote_address.sin_zero), 0, sizeof(remote_address.sin_zero));
    
    if((sock_to_server=socket(AF_INET, SOCK_STREAM,0))==-1){
        return -1; //safe exit
    }

    if(connect(sock_to_server, (struct sockaddr *) &remote_address, sizeof(struct sockaddr_in))==-1){
        //connection error
        printf("Server connection error\n");
        sigint_triggered=1;
    }


    while(sigint_triggered==0) {
        printf("\nEnter your message: ");
        fflush(stdout);
        size_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        buffer[bytes_read] = '\0';
        size_t bytes_sent = send(sock_to_server, buffer, bytes_read+1, 0);
    }
    // Cleanup
    if(sock_to_server!=-1){
        close(sock_to_server);
    }
    return 0;
}
