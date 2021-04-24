#include "proj.h"

struct send_msg msg;
struct sockaddr_in sockaddr, client;
struct data_msg rcvdata;
struct resp_msg resp;

int main (int argc, char* argv[]) {

    char* address;
    int port;
    int sfd;
    int ptonVal;
    socklen_t sockaddrLength;
    int fd;
    char* type;
    char* filename;


    if (argc != 5) {
        printf("Correct usage: ./server (address) (port) (-s or -r) (file_name\n");
        exit(1);
    }

    

    address = argv[1];
    port = atoi(argv[2]);
    type = argv[3];
    filename = argv[4];

    // Add fd = open() for filename, make sure to do O_CREAT | O_RDWR, 0644


    if (port < 1024 || port > 64000) {
        printf("Error: Port must be between 1024 and 64,000\n");
        exit(1);
    }

    ptonVal = inet_pton(AF_INET, address, &sockaddr.sin_addr.s_addr);
    if (ptonVal == 0) {
        printf("Error: address is not a valid character string representing a network address!\n");
        exit(1);
    } else if (ptonVal == -1) {
        perror("inet_pton");
        exit(1);
    }

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sfd == -1) {
        perror("socket");
        exit(1);
    }

    sockaddrLength = sizeof(sockaddr);

    if ((connect(sfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr))) == -1) {
        perror("connect");
        exit(1);
    }

    

    printf("Client: Connected to %s, port %d\n", address, port);
    printf("Client file name: %s\n", filename);

    if ((strcmp(type, "-s")) == 0) {
        msg.msg_type = 1;
        msg.file_size = 1; // Make this size of file in fd
        strcpy(msg.filename, filename);
        send_mesg(sfd, msg);
        printf("Client sends command to server: type %d, filesize %d, filename %s\n", msg.msg_type, msg.file_size, msg.filename); 
        resp = receive_resp(sfd, resp);
        printf("Client: response received, type = %d, status = %d, filesize = %d\n", resp.msg_type, resp.status, resp.filesize);
        send_data(fd, sfd);
        close(fd);
    } else if ((strcmp(type, "-r")) == 0) {
        msg.msg_type = 2;
        msg.file_size = 1; // Make this size of file in fd
        strcpy(msg.filename, filename);
        send_mesg(sfd, msg);
        printf("Client sends command to server: type %d, filesize %d, filename %s\n", msg.msg_type, msg.file_size, msg.filename); 
        resp = receive_resp(sfd, resp);
        printf("Client: response received, type = %d, status = %d, filesize = %d\n", resp.msg_type, resp.status, resp.filesize);
        rcvdata = recv_data(fd, sfd, rcvdata);
        close(fd);
    }
    
    
    
    close(sfd);
}
    