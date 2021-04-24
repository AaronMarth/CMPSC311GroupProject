#include "proj.h"


struct send_msg receive_msg(int afd, struct send_msg msg) {
    if ((recv(afd, (struct send_msg *)&msg, sizeof(msg), 0)) == -1) {
        perror("recv");
        close(afd);
        exit(0);
    
    }
    return(msg);
}

void send_mesg(int sfd, struct send_msg msg) {
    if ((send(sfd, (void*) &msg, sizeof(msg), 0)) == -1) {
        perror("send");
        close(sfd);
        exit(0);
    }
    printf("send_mesg sd = %d, leng = %lu\n", sfd, sizeof(msg));
}

struct resp_msg receive_resp(int sfd, struct resp_msg resp) {
    if ((recv(sfd, (struct resp_msg *)&resp, sizeof(resp), 0)) == -1) {
        perror("recv");
        close(sfd);
        exit(0);
    
    }
    return(resp);
}

void send_resp (int sfd, struct resp_msg resp) {
    if ((send(sfd, (void*) &resp, sizeof(resp), 0)) == -1) {
        perror("send");
        close(sfd);
        exit(0);
    }
    printf("Server sends response: type %d, status %d, and file size %d\n", resp.msg_type, resp.status, resp.filesize);
}


void send_data(int fd, int sfd) {
    int bytesSent = 0;
    struct data_msg sendmsg;
    sendmsg.msg_type = 4;
    while ((sendmsg.data_leng = read(fd, sendmsg.buffer, 1023)) != 0) {
        sendmsg.buffer[sendmsg.data_leng] = 0;
        if ((send(sfd, (void*) &sendmsg, sizeof(sendmsg), 0)) == -1) {
            perror("send");
            close(fd);
            close(sfd);
            exit(0);
        }
        printf("send_data sends %d bytes\n", sendmsg.data_leng);
        bytesSent += (int) sendmsg.data_leng;

    }
    printf("%d bytes successfully sent\n", bytesSent);
}

struct data_msg recv_data(int fd, int sfd, struct data_msg rcvdata) {
    int bytesRecieved;
    int totalBytes = 0;
    printf("receive_data: fd = %d, sd = %d, size = %d\n", fd, sfd, rcvdata.data_leng);
    while ((bytesRecieved = recv(sfd, (struct send_data *)&rcvdata, sizeof(rcvdata), 0)) != 0) {
        totalBytes += (int) rcvdata.data_leng;
        printf("receive_data writes %d bytes to file (total = %d)\n", rcvdata.data_leng, totalBytes);
        write(fd, rcvdata.buffer, rcvdata.data_leng);
    }
    return(rcvdata);
}


