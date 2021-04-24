struct send_msg {
    int msg_type;
    int file_size;
    char filename[128];
};

struct resp_msg {
    int msg_type;
    int status;
    int filesize;
};

struct data_msg {
    int msg_type;
    int data_leng;
    char buffer[1024];
};

struct send_msg receive_msg(int afd, struct send_msg send_msg);

struct resp_msg receive_resp(int sfd, struct resp_msg resp_msg);

void send_resp(int sfd, struct resp_msg);

void send_mesg(int sfd, struct send_msg);

void send_data(int fd, int sfd);

struct data_msg recv_data(int fd, int afd, struct data_msg rcvdata);