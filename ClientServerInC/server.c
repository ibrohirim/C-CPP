#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdbool.h>


void *connection_handler(void *socket_desc);

int main(){

    int welcomeSocket, newSocket, *second_sock;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(8000);
  /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*---- Bind the address struct to the socket ----*/
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));


    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5)==0)
        printf("Listening\n");
    else
        printf("Error\n");

    /*---- Accept call creates a new socket for the incoming connection ----*/
        addr_size = sizeof serverStorage;

    while((newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size))) {

        pthread_t sniffer_thread;
        second_sock = malloc(1);
        *second_sock = newSocket;

        if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) second_sock) < 0) {

            perror("Could not create thread");

            return 1;

        }

    }

    if(newSocket < 0 ){

        perror("Accept failed");
        return 1;

    }

    return 0;
}


void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];

    struct stat st;

    //Receive a message from client
    read_size = recv(sock , client_message , 2000 , 0);

    FILE *f = fopen(client_message, "rb");

    if(f == NULL) {

        char error[100] = "The file you requested does not exist\n";

        send(sock, error, 100, 0);

        return 0;

    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    stat(client_message, &st);
    int fileSize = st.st_size;

    char *contents = malloc(fsize + 1);
    fread(contents, fsize, 1, f);
    fclose(f);

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    send(sock, (const char*)&fileSize, 4, 0);

    send(sock, contents, fsize, 0);

    //write(sock, contents, strlen(contents));

    free(contents);

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
