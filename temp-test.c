#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define LISTENADDR "0.0.0.0"

int server_init()

int main(int argc,char &argv[])
{
    int s;
    char *port;

    if(argc < 2)
    {
        fprintf(stderr,"Usage : %s <listenning port>\n")
    }

    s = server_init();
}