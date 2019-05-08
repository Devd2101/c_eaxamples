#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET_NAME "/tmp/demoSock"
#define SIZE 1024

struct sockaddr_un{
  sa_family_t sun_family;
  char sun_path[108];
};

int main(){
  int master_socket, data_socket;
  struct sockaddr_un server;
  unsigned int len;
  int ret;
  int result;
  int data;
  char buffer[SIZE];
  char msg[] ="Hello welcome to socket world!!!";
  printf("%s", msg);
// create a socket using socket syscall.
  master_socket = socket(AF_UNIX,SOCK_STREAM,0);
  if(master_socket== -1){
    perror("Socket creation error\n");
    exit(1);
  }
memset(&server, 0, sizeof(struct sockaddr_un));
server.sun_family = AF_UNIX;
strncpy(server.sun_path,SOCKET_NAME,sizeof(server.sun_path)-1);
// bind the host and port to socket.
ret = bind(master_socket,(struct sockaddr *)&server,sizeof(struct sockaddr_un));

if(ret == -1){
  perror("bind");
  exit(1);
}
//listen to connection.

ret = listen(master_socket, 10);
if(ret ==-1)
{
  perror("listen");
  exit(1);
}

// accept the incomming connection 
for(;;){
  printf("waiting for incoming connection");
  data_socket = accept(master_socket,NULL,NULL);
  if(data_socket == -1){
    perror("accept");
    exit(EXIT_FAILURE);
  }
  
  result = 0;
  for(;;){
    memset(buffer,0, SIZE);
    printf("waiting for data from client\n");
    ret = read(data_socket,buffer, SIZE);
    if(ret == -1){
      perror("read");
      exit(EXIT_FAILURE);
    }
    memcpy(&data, buffer, sizeof(int));
    if(data == 0)break;
    result += data;
  }
  memset(buffer, 0, SIZE);
  sprintf(buffer,"result is %d\n",result);
  ret = write(data_socket,buffer,SIZE);
  if(ret == -1){
    perror("write");
    exit(EXIT_FAILURE);
  }
  close(data_socket);
}
close(master_socket);
unlink(SOCKET_NAME);

exit(0);
}
