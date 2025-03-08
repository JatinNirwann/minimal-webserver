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
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    

    bind(socket_info,(struct sockaddr*)&addr,sizeof(addr));

    listen(socket_info,10);//10 is the max client limit after that new clients willl be rejected
    
    while(1)
    {   int client_fd = accept(socket_info,0,0);

        char buffer[256] = {0};
        recv(client_fd,buffer,256,0);

    //    GET /file.html ;

        char* f = buffer +5;
        *strchr(f,' ') = 0;
        int opened_fd = open(f,O_RDONLY);

        // Send HTTP response header
        char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(client_fd, header, strlen(header), 0);
        
        off_t file_size = lseek(opened_fd, 0, SEEK_END);
        lseek(opened_fd, 0, SEEK_SET);  // Reset file position
        
        // Send the entire file
        off_t offset = 0;
        sendfile(client_fd, opened_fd, &offset, file_size);        

        close(opened_fd);
        close(client_fd);
    };
    close(socket_info);

    return 0;
}