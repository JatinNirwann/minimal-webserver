
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

int main()
{
    int socket_info = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(socket_info, (struct sockaddr*)&addr, sizeof(addr));
    listen(socket_info, 10);
    
    printf("Server running on port 8080...\n");
    
    while(1)
    {   
        int client_fd = accept(socket_info, NULL, NULL);
        
        char buffer[4096] = {0};
        recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        
        // prasing http req
        char *request_line = strtok(buffer, "\r\n");
        char *path = request_line + 4; // to skip get
        char *space = strchr(path, ' ');
        if (space) *space = '\0';
        
        // default html fiile
        if (strcmp(path, "/") == 0) {
            path = "/file.html";
        }
        
        // removing leading slash and open file
        char *filepath = path + 1;
        int opened_fd = open(filepath, O_RDONLY);
        
        if (opened_fd == -1) {
            // 404
            char not_found[] = "HTTP/1.1 404 Not Found\r\n\r\nFile not found";
            send(client_fd, not_found, strlen(not_found), 0);
            close(client_fd);
            continue;
        }
        
        
        struct stat file_stat;
        fstat(opened_fd, &file_stat);
        
        // checking file type using file extention
        char *content_type = "text/plain";
        char *ext = strrchr(filepath, '.');
        if (ext) {
            if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) {
                content_type = "text/html";
            } else if (strcmp(ext, ".css") == 0) {
                content_type = "text/css";
            } else if (strcmp(ext, ".js") == 0) {
                content_type = "application/javascript";
            } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
                content_type = "image/jpeg";
            } else if (strcmp(ext, ".png") == 0) {
                content_type = "image/png";
            }
        }
        
        // response
        char header[512];
        sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n", 
                content_type, file_stat.st_size);
        send(client_fd, header, strlen(header), 0);
        
        // sending file contents
        off_t offset = 0;
        sendfile(client_fd, opened_fd, &offset, file_stat.st_size);
        
        close(opened_fd);
        close(client_fd);
    }
    
    close(socket_info);
    return 0;
}