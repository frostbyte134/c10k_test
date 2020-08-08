#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include "c10k_msg_proc.h"


int send_message(int sock_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size){
    int ret = -1; 
}

int recv_message(int sock_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size){
    int ret = -1;
    ret = read(sock_fd, buffer, BUF_SIZE);
    return ret;
}
