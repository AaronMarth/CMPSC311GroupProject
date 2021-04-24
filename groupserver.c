
#include "proj.h"


struct sockaddr_in sockaddr, client;
struct send_msg msg;
struct data_msg rcvdata;
struct resp_msg resp;

int main (int argc, char * argv[]) {
    char buff[max_data_size];
    char* address;
    int port;
    int ptonVal;
    int sfd;
    int fileSize;
    int afd;
    int fd;

    if (argc != 3) {
        printf("Correct usage: ./server (address) (port)\n");
        exit(1);
    }

    address = argv[1];
    port = atoi(argv[2]);

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
    

    if (bind(sfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    

    int sockaddrLength = sizeof(client);

    while (1) {

        
        printf("Server: listening\n");

        if ((listen(sfd, 10)) != 0) {
            perror("listen");
            close(sfd);
            exit(1);
        }
        
        if ((afd = accept(sfd, (struct sockaddr *) &client, &sockaddrLength)) == -1) {
            perror("accept");
            close(afd);
            exit(0);
        }

        printf("Server: Connected to %s, port %d\n", address, port);
        
        msg = receive_msg(afd, msg);

        if (msg.msg_type == 1) {
            printf("Sever: received message type %d from %s, port %d\n", msg.msg_type, address, port);
            resp.msg_type = msg.msg_type;
            resp.status = 0;
            resp.filesize = 0;
            send_resp(afd, resp);
            printf("Server awaits data\n");
            fd = open(msg.filename, O_RDWR | O_CREAT, 0644);
            rcvdata = recv_data(fd, afd, rcvdata);
            close(fd);
            printf("To server data transfer succeeded\n");
        } else if (msg.msg_type == 2) {
            printf("Sever: received message type %d from %s, port %d\n", msg.msg_type, address, port);
            printf("Server: file name: %s\n", msg.filename);
            fd = open(msg.filename, O_RDWR | O_CREAT | 0644);
            send_data(fd, afd);
            close(fd);

        }
        


    

        close(afd);
    }
}