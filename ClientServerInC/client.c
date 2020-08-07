#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
  int clientSocket;
  char address[256];
  char fileName[256];
  char port[256];
  int fileSize;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  printf("Please input the IP address of the machine you wish to connect to: ");
  scanf("%s", address);

  printf("Please enter a port number: ");
  scanf("%s", port);

  int portNumber = atoi(port);

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(portNumber);
  /* Set IP address to localhost */

  serverAddr.sin_addr.s_addr = inet_addr(address);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0 ) {

    printf("Connection failed\n");
    return 0;

  } else {

    printf("Connection established\n");

  }

  printf("Please enter the name of the file you wish to receive: ");
  scanf("%s", fileName);

  if(send(clientSocket, fileName, strlen(fileName), 0) < 0 ) {

    printf("Send failed");
    return 1;

  }

  /*---- Read the message from the server into the buffer ----*/

  recv(clientSocket, &fileSize, 4, 0);

  printf("size of file: %d\n", fileSize);

  char buffer[fileSize+1];

  recv(clientSocket, buffer, fileSize, 0);

  /*---- Print the received message ----*/
  printf("Data received: \n%s",buffer);

  return 0;
}
