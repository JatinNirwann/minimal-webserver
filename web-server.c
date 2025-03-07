#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h> //for open func
#include <unistd.h>
#include <sys/sendfile.h> 


int main()
{
    int socket_info = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr = {
        AF_INET,  // ipv4
        0x901f,   // Port in hexadecimak
        // hexadecimal of 8080 is 1F90 but its written like this swapping first two with last two
        0         // default
    };
    

    bind(socket_info,(struct sockaddr*)&addr,sizeof(addr));

    listen(socket_info,10);//10 is the max client limit after that new clients willl be rejected
    int client_fd = accept(socket_info,0,0);

    char buffer[256] = {0};
    recv(client_fd,buffer,256,0);

//    GET /file.html ;

    char* f = buffer +5;
    *strchr(f,' ') = 0;
    int opened_fd = open(f,O_RDONLY);
    sendfile(client_fd,opened_fd,0,256);
    close(opened_fd);
    close(client_fd);
    close(socket_info);

    return 0;
}