#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include "c10k_epoll.h"

#define EPOLL_SIZE (20000u)
struct epoll_event events[EPOLL_SIZE]; // occured events

void epoll_work_forever(int serv_fd, uint8_t buffer[BUF_SIZE]){
    FILE* log_file = fopen("log.ini", "wt");

    struct epoll_event ev;  // ev : for main epoll control?
    
    
    struct sockaddr_in client_addr;
    
    int fd_max = serv_fd; //for each connected clients, assign socket file descriptor
    int fd_num = 0;
    printf("[SERV][EPOLL]: wating connection request.\n");
    unsigned int cnt = 0;
    
    int epollfd;
    if((epollfd = epoll_create(EPOLL_SIZE)) == -1){
        fprintf(stderr, "[SERV][EPOLL] cannot create epoll with size %u", EPOLL_SIZE);
        exit(-1);
    }
    printf("[SERV][EPOLL]: epoll_create success\n");

    ev.events = EPOLLIN;
    ev.data.fd = serv_fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_fd, &ev);
    printf("[SERV][EPOLL]: epoll_ctl success\n");

    while(1)
    {
        cnt++;
        int event_size =  epoll_wait(epollfd, events, EPOLL_SIZE, -1); 
        if(cnt % 1000 == 0){
            //printf("event size = %d\n", event_size);
        }
        if(event_size == -1)
        {
            printf("[SERV][EPOLL]: event_size -1, terminating\n");
            exit(-1);
        }
        for(int i = 0; i<event_size; i++){
            if(events[i].data.fd == serv_fd){
                int sock_fd = 0, add_struct_size = 0;
                sock_fd = accept(serv_fd, (struct sockaddr *)&client_addr, &add_struct_size);
                ev.events = EPOLLIN;
                ev.data.fd = sock_fd;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_fd, &ev);
                printf("[SERV][EPOLL] accepting new socket %d\n", sock_fd);
            }else{
                //if(cnt % 100000 == 0)
                //    printf("[SERV][EPOLL] processed socket %d\n", i);
                int msg_size = read(events[i].data.fd, buffer, BUF_SIZE);
                if(msg_size == 0){
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, events);
                    close(events[i].data.fd);
                    //printf("[SERV][EPOLL] Server : %d client closed.\n", i);
                }
            }
        }
    }
    close(serv_fd);
    fclose(log_file);
    printf("[SERV][EPOLL], closed socket successfully (unreachable, perahps)\n");
}